import asyncio
import time
from typing import Dict, Any, Optional, Tuple

import httpx
from bs4 import BeautifulSoup
from urllib.parse import urlparse
from robotexclusionrulesparser import RobotExclusionRulesParser

from database import Database
import utils


class Crawler:
    def __init__(self, config: Dict[str, Any], db: Database):
        self.config = config
        self.logic = config["logic"]
        self.db = db

        self.user_agent = self.logic.get("user_agent", "MAI-IR-Bot/1.0")
        self.headers = {"User-Agent": self.user_agent}
        self.client = httpx.AsyncClient(headers=self.headers, follow_redirects=True, timeout=20.0)

        self.base_delay_sec = float(self.logic.get("base_delay_sec", 1.0))
        self.sleep_on_empty_sec = int(self.logic.get("sleep_on_empty_frontier_sec", 10))
        self.re_crawl_interval_sec = int(self.logic.get("re_crawl_interval_sec", 86400))
        self.retry_backoff_sec = int(self.logic.get("retry_backoff_sec", 300))
        self.max_retries = int(self.logic.get("max_retries", 3))
        self.max_pages_total = int(self.logic.get("max_pages_total", 0))  # 0 => unlimited
        self.send_conditional = bool(self.logic.get("send_conditional_headers", True))
        self.store_all_pages = bool(self.logic.get("store_all_pages", True))

        self.allowed_domains = set(self.logic.get("allow_domains", []))
        if not self.allowed_domains:
            self.allowed_domains = {utils.get_domain(u) for u in self.logic.get("seed_urls", [])}

        rules_cfg = self.logic.get("rules", {})
        self.domain_rules: dict[str, dict[str, list]] = {}
        for domain, rcfg in rules_cfg.items():
            self.domain_rules[domain] = {
                "allow": utils.compile_regex_list(rcfg.get("allow_regex")),
                "deny": utils.compile_regex_list(rcfg.get("disallow_regex")),
            }

        self.store_allow = utils.compile_regex_list(self.logic.get("store_allow_regex"))

        self.robots_cache: Dict[str, RobotExclusionRulesParser] = {}
        self.host_last_access: Dict[str, float] = {}
        self.pages_processed = 0

        print(f"[Crawler] Allowed domains: {sorted(self.allowed_domains)}")


    async def _get_robots(self, url: str) -> RobotExclusionRulesParser:
        host_root = urlparse(url)._replace(path="", params="", query="", fragment="").geturl()
        robots_url = f"{host_root}/robots.txt"

        if host_root not in self.robots_cache:
            parser = RobotExclusionRulesParser()
            try:
                print(f"[Robots] Fetching {robots_url} ...")
                resp = await self.client.get(robots_url)
                if resp.status_code == 200:
                    parser.parse(resp.text)
                else:
                    parser.allow_all = True
            except Exception:
                parser.allow_all = True
            self.robots_cache[host_root] = parser

        return self.robots_cache[host_root]


    async def _apply_politeness(self, url: str):
        host = urlparse(url).netloc
        last_access = self.host_last_access.get(host, 0.0)
        now = time.time()
        dt = now - last_access
        if dt < self.base_delay_sec:
            sleep_time = self.base_delay_sec - dt
            await asyncio.sleep(sleep_time)
        self.host_last_access[host] = time.time()


    def _is_allowed_by_rules(self, url: str) -> bool:
        domain = utils.get_domain(url)
        if domain not in self.allowed_domains:
            return False

        rule = self.domain_rules.get(domain)
        if not rule:
            return True

        deny = rule["deny"]
        allow = rule["allow"]
        if deny and utils.matches_any(deny, url):
            return False
        if allow:
            return utils.matches_any(allow, url)
        return True


    def _should_store(self, url: str) -> bool:
        if self.store_all_pages:
            return True
        if not self.store_allow:
            return True
        return utils.matches_any(self.store_allow, url)


    async def fetch_page(self, url: str) -> Tuple[Optional[str], Optional[str], Optional[str], str]:
        try:
            if utils.is_media_file(url):
                self.db.mark_skipped(url, "media_file")
                return None, None, None, "skipped_media"

            if not self._is_allowed_by_rules(url):
                self.db.mark_skipped(url, "disallowed_by_rules")
                return None, None, None, "skipped_rules"

            robots = await self._get_robots(url)
            if not robots.is_allowed(self.user_agent, url):
                self.db.mark_skipped(url, "robot_disallowed")
                return None, None, None, "skipped_robots"

            await self._apply_politeness(url)

            req_headers = dict(self.headers)
            if self.send_conditional:
                meta = self.db.get_doc_meta(url)
                if meta.get("etag"):
                    req_headers["If-None-Match"] = meta["etag"]
                if meta.get("last_modified"):
                    req_headers["If-Modified-Since"] = meta["last_modified"]

            print(f"[Fetch] {url}")
            resp = await self.client.get(url, headers=req_headers)

            if resp.status_code == 304:
                # unchanged
                self.db.upsert_document(
                    url=url,
                    raw_html=None,
                    source_name=utils.get_source_name(url),
                    content_hash=None,
                    etag=resp.headers.get("ETag"),
                    last_modified=resp.headers.get("Last-Modified"),
                    changed=False,
                )
                self.db.mark_done(url)
                return None, resp.headers.get("ETag"), resp.headers.get("Last-Modified"), "not_modified"

            if resp.status_code != 200:
                self.db.mark_failed(url, f"http_{resp.status_code}")
                return None, None, None, f"failed_{resp.status_code}"

            ctype = resp.headers.get("Content-Type", "")
            if "text/html" not in ctype:
                self.db.mark_skipped(url, f"non_html_{ctype[:30]}")
                return None, None, None, "skipped_non_html"

            return resp.text, resp.headers.get("ETag"), resp.headers.get("Last-Modified"), "ok"

        except Exception as e:
            self.db.mark_failed(url, f"exception_{type(e).__name__}")
            return None, None, None, "failed_exception"


    def parse_links(self, base_url: str, raw_html: str) -> tuple[int, int]:
        soup = BeautifulSoup(raw_html, "lxml")
        found = 0
        added = 0

        for a in soup.find_all("a", href=True):
            href = a.get("href")
            if not href:
                continue

            normalized = utils.normalize_url(base_url, href)
            if not normalized:
                continue

            found += 1
            if not self._is_allowed_by_rules(normalized):
                continue
            if self.db.add_to_frontier(normalized):
                added += 1

        return found, added


    def store_document_if_needed(
        self,
        url: str,
        raw_html: str,
        etag: Optional[str],
        last_modified: Optional[str],
    ) -> bool:
        if not self._should_store(url):
            self.db.mark_done(url)
            return False

        new_hash = utils.hash_content(raw_html)
        old_hash = self.db.get_doc_meta(url).get("content_hash")
        changed = new_hash != old_hash

        self.db.upsert_document(
            url=url,
            raw_html=raw_html,
            source_name=utils.get_source_name(url),
            content_hash=new_hash,
            etag=etag,
            last_modified=last_modified,
            changed=changed,
        )

        self.db.mark_done(url)
        if changed:
            print(f"[DB] Updated: {url} ({new_hash[:8]}...) ")
        else:
            print(f"[DB] Unchanged (hash): {url}")
        return changed


    async def run(self):
        self.db.reset_processing_to_pending()
        seeds = []
        for u in self.logic.get("seed_urls", []):
            nu = utils.normalize_abs_url(u)
            if nu:
                seeds.append(nu)
        self.db.add_seed_urls(seeds)

        while True:
            if self.max_pages_total > 0 and self.pages_processed >= self.max_pages_total:
                print(f"[Crawler] Reached max_pages_total={self.max_pages_total}. Stop.")
                return

            next_item = self.db.get_next_url_to_crawl(
                re_crawl_interval_sec=self.re_crawl_interval_sec,
                retry_backoff_sec=self.retry_backoff_sec,
                max_retries=self.max_retries,
            )
            if not next_item:
                await asyncio.sleep(self.sleep_on_empty_sec)
                continue

            url = next_item["url"]
            html, etag, last_mod, status = await self.fetch_page(url)
            self.pages_processed += 1

            if status == "ok" and html:
                # store/update
                self.store_document_if_needed(url, html, etag, last_mod)

                # discover
                found, added = self.parse_links(url, html)
                if found:
                    print(f"[Parse] links: found={found} added={added}")

