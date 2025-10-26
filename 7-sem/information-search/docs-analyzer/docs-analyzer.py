#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Multi-source Corpus Analyzer (Habr + StackOverflow RU)
- Собирает статьи/вопросы с двух источников
- Сохраняет raw HTML и очищенный текст
- Считает статистику per-source и общую

Зависимости: requests, beautifulsoup4, lxml, readability-lxml, tqdm
"""

import argparse
import hashlib
import html
import json
import random
import re
import sys
import time
from pathlib import Path
from urllib.parse import urljoin
from urllib import robotparser

import requests
from bs4 import BeautifulSoup
from readability import Document
from tqdm import tqdm


USER_AGENT = "CorpusAnalyzer/1.2 (+edu; contact: you@example.com)"

SOURCES = {
    "habr": {
        "domain": "habr.com",
        "seeds": [
            "https://habr.com/ru/all/",
            "https://habr.com/ru/articles/",
        ],
        # Ссылки-статьи вида /ru/articles/123456/
        "link_re": r"^https?://habr\.com/ru/articles/\d+/?$",
        "readability_first": False,
        "css_title": [
            "h1.tm-title_h1",
            "h1.article-formatted-header__title",
            "h1.tm-article-snippet__title_h1",
        ],
        "css_body": [
            "div.tm-article-presenter__body",
            "div#post-content-body",
            "div.article-formatted-body",
            "article div.content__body",
        ],
    },
    "soru": {
        "domain": "ru.stackoverflow.com",
        "seeds": [
            "https://ru.stackoverflow.com/questions?tab=Newest",
            "https://ru.stackoverflow.com/questions?tab=Active",
        ],
        # Вопросы вида /questions/NNNNNN[/slug]
        "link_re": r"^https?://ru\.stackoverflow\.com/questions/\d+(?:/[^/?#]*)?/?$",
        # На SO удобнее readability, CSS — как fallback
        "readability_first": True,
        "css_title": [
            "h1",
            "h1 a.question-hyperlink",
        ],
        # Берём основной текст вопроса; ответы можно добавить по желанию
        "css_body": [
            "div#question .s-prose",
            "div#question .js-post-body",
            "div.postcell .post-text",   # старый шаблон
        ],
    },
}


def parse_args():
    ap = argparse.ArgumentParser(description="Multi-source Corpus Analyzer (Habr + StackOverflow RU)")
    ap.add_argument("--sources", nargs="*", default=["habr", "soru"], choices=list(SOURCES.keys()),
                    help="Какие источники собирать (по умолчанию: habr soru)")
    ap.add_argument("--pages", type=int, default=20, help="Максимум страниц листинга на seed")
    ap.add_argument("--limit-per-source", type=int, default=300, help="Предел документов на источник")
    ap.add_argument("--out", type=Path, default=Path("data_habr_soru"), help="Каталог для сохранения")
    ap.add_argument("--delay", type=float, default=1.8, help="Задержка между запросами (сек)")
    ap.add_argument("--timeout", type=float, default=15.0, help="HTTP таймаут (сек)")
    ap.add_argument("--respect_robots", action="store_true", help="Проверять robots.txt")
    return ap.parse_args()


def ensure_dirs(base: Path, src: str):
    (base / src / "raw").mkdir(parents=True, exist_ok=True)
    (base / src / "text").mkdir(parents=True, exist_ok=True)
    (base / src / "meta").mkdir(parents=True, exist_ok=True)


def sha1(s: str) -> str:
    return hashlib.sha1(s.encode("utf-8")).hexdigest()


def build_session(timeout: float):
    s = requests.Session()
    s.headers.update({"User-Agent": USER_AGENT, "Accept-Language": "ru,en;q=0.9"})
    s.timeout = timeout
    return s


def load_robots(domain: str) -> robotparser.RobotFileParser:
    robots_url = f"https://{domain}/robots.txt"
    rp = robotparser.RobotFileParser()
    try:
        rp.set_url(robots_url)
        rp.read()
    except Exception:
        pass
    return rp


def can_fetch(rp: robotparser.RobotFileParser, url: str) -> bool:
    try:
        return rp.can_fetch(USER_AGENT, url)
    except Exception:
        return False


def iter_listing_urls(seed: str, pages: int):
    """Пробуем /page{i}/ и ?page=i — что сработает."""
    seed = seed.rstrip("/")
    yield seed
    yield seed + "/"  # на всякий случай
    for i in range(2, pages + 1):
        yield f"{seed}/page{i}/"
        sep = "&" if "?" in seed else "?"
        yield f"{seed}{sep}page={i}"


def extract_links(html_text: str, base_url: str, link_re: re.Pattern):
    soup = BeautifulSoup(html_text, "lxml")
    links = set()
    for a in soup.find_all("a", href=True):
        url = urljoin(base_url, a["href"]).split("#")[0]
        # уберём типичные UTM-хвосты
        url = re.sub(r"[?&]utm_[^=&]+=[^&]+", "", url)
        if link_re.match(url):
            links.add(url.rstrip("/"))
    return sorted(links)


def fetch(session: requests.Session, url: str, timeout: float):
    r = session.get(url, timeout=timeout)
    r.raise_for_status()
    return r


def save_bytes(path: Path, data: bytes):
    path.write_bytes(data)


def save_text(path: Path, text: str):
    path.write_text(text, encoding="utf-8")


def pick_first(soup: BeautifulSoup, selectors):
    for sel in selectors or []:
        node = soup.select_one(sel)
        if node and node.get_text(strip=True):
            return node.get_text(" ", strip=True)
    return None


def extract_with_css(html_text: str, css_title, css_body):
    soup = BeautifulSoup(html_text, "lxml")
    title = pick_first(soup, css_title)
    body_parts = []
    for sel in css_body or []:
        for n in soup.select(sel):
            txt = n.get_text(" ", strip=True)
            if txt:
                body_parts.append(txt)
    body = "\n\n".join(body_parts).strip() if body_parts else None
    return title, body


def extract_with_readability(url: str, html_text: str):
    doc = Document(html_text)
    title = html.unescape(doc.short_title() or "") or None
    summary_html = doc.summary(html_partial=True)
    soup = BeautifulSoup(summary_html or "", "lxml")
    body = soup.get_text(" ", strip=True) if summary_html else None
    return title, body


def extract_text(url: str, html_text: str, readability_first=False, css_title=None, css_body=None):
    if readability_first:
        t, b = extract_with_readability(url, html_text)
        if not b:
            t2, b2 = extract_with_css(html_text, css_title, css_body)
            t = t or t2
            b = b or b2
    else:
        t, b = extract_with_css(html_text, css_title, css_body)
        if not b:
            t2, b2 = extract_with_readability(url, html_text)
            t = t or t2
            b = b or b2
    return (t or "").strip(), (b or "").strip()


def word_count(text: str) -> int:
    return len(re.findall(r"\w+", text, flags=re.UNICODE))


def compute_stats(meta_rows):
    n = len(meta_rows)
    total_raw = sum(m.get("bytes_raw", 0) for m in meta_rows)
    total_txt = sum(m.get("bytes_text", 0) for m in meta_rows)
    stats = {
        "documents": n,
        "bytes_raw_total": total_raw,
        "bytes_text_total": total_txt,
        "avg_bytes_raw": total_raw / n if n else 0,
        "avg_bytes_text": total_txt / n if n else 0,
    }
    words = sorted([m.get("words", 0) for m in meta_rows])
    if words:
        def pct(p):
            k = int((p/100.0) * (len(words) - 1))
            return words[k]
        stats.update({
            "words_min": words[0],
            "words_p50": pct(50),
            "words_p90": pct(90),
            "words_p99": pct(99),
            "words_max": words[-1],
        })
    return stats


def print_stats(stats):
    print(f"Документов:                 {stats['documents']}")
    print(f"Общий размер 'сырья':       {stats['bytes_raw_total']/1024/1024:.2f} МБ")
    print(f"Общий размер чистого текста:{stats['bytes_text_total']/1024/1024:.2f} МБ")
    print(f"Средний размер 'сырья':     {stats['avg_bytes_raw']/1024:.1f} КБ/док")
    print(f"Средний размер текста:      {stats['avg_bytes_text']/1024:.1f} КБ/док")
    if "words_min" in stats:
        print(f"Слова (min/p50/p90/p99/max): {stats['words_min']}/{stats['words_p50']}/{stats['words_p90']}/{stats['words_p99']}/{stats['words_max']}")


def write_combined_meta(out_dir: Path, all_meta):
    meta_dir = out_dir / "meta"
    meta_dir.mkdir(parents=True, exist_ok=True)
    combined_path = meta_dir / "meta_combined.jsonl"
    with combined_path.open("w", encoding="utf-8") as f:
        for row in all_meta:
            f.write(json.dumps(row, ensure_ascii=False) + "\n")
    stats = compute_stats(all_meta)
    with (meta_dir / "stats_combined.json").open("w", encoding="utf-8") as f:
        json.dump(stats, f, ensure_ascii=False, indent=2)
    print("\n=== COMBINED — сводка по обоим источникам ===")
    print_stats(stats)


def analyze_source(name: str, cfg: dict, args, session: requests.Session):
    domain = cfg["domain"]
    rp = load_robots(domain) if args.respect_robots else None
    ensure_dirs(args.out, name)
    link_re = re.compile(cfg["link_re"])

    # Сбор ссылок
    article_urls = []
    visited_listing = set()
    for seed in cfg["seeds"]:
        for lst_url in iter_listing_urls(seed, args.pages):
            if lst_url in visited_listing:
                continue
            visited_listing.add(lst_url)
            try:
                if rp and not can_fetch(rp, lst_url):
                    continue
                time.sleep(args.delay)
                r = fetch(session, lst_url, args.timeout)
                links = extract_links(r.text, lst_url, link_re)
                article_urls.extend(links)
            except Exception as e:
                print(f"[{name}:LIST] {lst_url} -> {e}", file=sys.stderr)

    # Дедуп и обрезка
    random.shuffle(article_urls)
    article_urls = sorted(set(article_urls))[: args.limit_per_source]

    total_raw = 0
    total_txt = 0
    meta_rows = []

    pbar = tqdm(article_urls, desc=f"{name}: fetch", unit="doc")
    for url in pbar:
        try:
            if rp and not can_fetch(rp, url):
                continue
            time.sleep(args.delay)
            r = fetch(session, url, args.timeout)
            raw_bytes = r.content
            raw_size = len(raw_bytes)
            total_raw += raw_size

            doc_id = sha1(url)
            raw_path = args.out / name / "raw" / f"{doc_id}.html"
            save_bytes(raw_path, raw_bytes)

            title, body = extract_text(
                url, r.text,
                readability_first=cfg.get("readability_first", False),
                css_title=cfg.get("css_title"),
                css_body=cfg.get("css_body"),
            )
            clean_text = (title + "\n\n" + body).strip() if (title or body) else ""
            text_size = len(clean_text.encode("utf-8"))
            total_txt += text_size

            text_path = args.out / name / "text" / f"{doc_id}.txt"
            save_text(text_path, clean_text)

            wc = word_count(clean_text)
            meta = {
                "id": doc_id,
                "source": name,
                "url": url,
                "status": r.status_code,
                "raw_path": str(raw_path),
                "text_path": str(text_path),
                "title": title,
                "bytes_raw": raw_size,
                "bytes_text": text_size,
                "words": wc,
            }
            meta_rows.append(meta)

            # прогресс
            if meta_rows:
                avg_raw = total_raw / len(meta_rows)
                avg_txt = total_txt / len(meta_rows)
                pbar.set_postfix({
                    "saved": len(meta_rows),
                    "avg_raw_kb": f"{avg_raw/1024:.1f}",
                    "avg_txt_kb": f"{avg_txt/1024:.1f}",
                })

        except Exception as e:
            print(f"[{name}:DOC] {url} -> {e}", file=sys.stderr)
            continue

    # Пер-источниковые метаданные и статистика
    meta_dir = args.out / name / "meta"
    meta_dir.mkdir(parents=True, exist_ok=True)
    with (meta_dir / "meta.jsonl").open("w", encoding="utf-8") as f:
        for row in meta_rows:
            f.write(json.dumps(row, ensure_ascii=False) + "\n")

    stats = compute_stats(meta_rows)
    with (meta_dir / "stats.json").open("w", encoding="utf-8") as f:
        json.dump(stats, f, ensure_ascii=False, indent=2)

    print(f"\n=== {name} — сводка ===")
    print_stats(stats)
    return meta_rows, stats


def main():
    args = parse_args()
    session = build_session(args.timeout)

    all_meta = []
    for name in args.sources:
        meta_rows, _ = analyze_source(name, SOURCES[name], args, session)
        all_meta.extend(meta_rows)

    write_combined_meta(args.out, all_meta)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n[INTERRUPTED] Завершение по Ctrl+C", file=sys.stderr)
        sys.exit(130)
