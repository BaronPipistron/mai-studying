import time
from typing import Optional, Dict, Any
from pymongo import MongoClient, ReturnDocument


class Database:
    def __init__(self, config: Dict[str, Any]):
        db_config = config["db"]
        self.client = MongoClient(db_config["connection_string"])
        self.db = self.client[db_config["database_name"]]
        self.doc_collection = self.db[db_config["doc_collection"]]
        self.frontier_collection = self.db[db_config["frontier_collection"]]

        self.doc_collection.create_index("url", unique=True)
        self.frontier_collection.create_index("url", unique=True)
        self.frontier_collection.create_index([
            ("status", 1),
            ("last_crawl_attempt", 1),
        ])


    def reset_processing_to_pending(self):
        res = self.frontier_collection.update_many(
            {"status": "processing"},
            {"$set": {"status": "pending"}},
        )
        if res.modified_count:
            print(f"[DB] Reset processing->pending: {res.modified_count}")


    def add_seed_urls(self, seed_urls: list[str]):
        for url in seed_urls:
            self.add_to_frontier(url)


    def add_to_frontier(self, url: str) -> bool:
        now_ts = int(time.time())
        res = self.frontier_collection.update_one(
            {"url": url},
            {
                "$setOnInsert": {
                    "url": url,
                    "status": "pending",
                    "last_crawl_attempt": None,
                    "first_seen": now_ts,
                    "retries": 0,
                }
            },
            upsert=True,
        )
        return res.upserted_id is not None


    def get_next_url_to_crawl(
        self,
        re_crawl_interval_sec: int,
        retry_backoff_sec: int,
        max_retries: int,
    ) -> Optional[Dict[str, Any]]:
        now_ts = int(time.time())
        doc = self.frontier_collection.find_one_and_update(
            {"status": "pending"},
            {"$set": {"status": "processing", "last_crawl_attempt": now_ts}},
            return_document=ReturnDocument.AFTER,
        )
        if doc:
            return doc

        doc = self.frontier_collection.find_one_and_update(
            {
                "status": {"$regex": r"^failed"},
                "retries": {"$lt": max_retries},
                "last_crawl_attempt": {"$lt": now_ts - retry_backoff_sec},
            },
            {
                "$set": {"status": "processing", "last_crawl_attempt": now_ts},
                "$inc": {"retries": 1},
            },
            sort=[("last_crawl_attempt", 1)],
            return_document=ReturnDocument.AFTER,
        )
        if doc:
            return doc

        doc = self.frontier_collection.find_one_and_update(
            {
                "status": "done",
                "last_crawl_attempt": {"$lt": now_ts - re_crawl_interval_sec},
            },
            {"$set": {"status": "processing", "last_crawl_attempt": now_ts}},
            sort=[("last_crawl_attempt", 1)],
            return_document=ReturnDocument.AFTER,
        )
        return doc


    def mark_frontier(self, url: str, status: str, *, error: Optional[str] = None):
        now_ts = int(time.time())
        update: Dict[str, Any] = {"status": status, "last_crawl_attempt": now_ts}
        if error:
            update["error"] = error
        else:
            update["error"] = None

        self.frontier_collection.update_one(
            {"url": url},
            {"$set": update},
            upsert=True,
        )


    def get_doc_meta(self, url: str) -> Dict[str, Optional[str]]:
        doc = self.doc_collection.find_one(
            {"url": url},
            {"content_hash": 1, "etag": 1, "last_modified": 1},
        )
        if not doc:
            return {"content_hash": None, "etag": None, "last_modified": None}
        return {
            "content_hash": doc.get("content_hash"),
            "etag": doc.get("etag"),
            "last_modified": doc.get("last_modified"),
        }


    def upsert_document(
        self,
        url: str,
        raw_html: Optional[str],
        source_name: str,
        content_hash: Optional[str],
        etag: Optional[str],
        last_modified: Optional[str],
        *,
        changed: bool,
    ):
        now_ts = int(time.time())

        update: Dict[str, Any] = {
            "url": url,
            "source_name": source_name,
            "last_checked": now_ts,
            "etag": etag,
            "last_modified": last_modified,
        }

        if changed:
            update.update(
                {
                    "raw_html": raw_html,
                    "crawl_date": now_ts,
                    "content_hash": content_hash,
                }
            )

        self.doc_collection.update_one({"url": url}, {"$set": update}, upsert=True)


    def mark_done(self, url: str):
        self.mark_frontier(url, "done")


    def mark_failed(self, url: str, reason: str):
        safe_reason = reason.replace(" ", "_")[:50]
        self.mark_frontier(url, f"failed_{safe_reason}", error=reason)


    def mark_skipped(self, url: str, reason: str):
        safe_reason = reason.replace(" ", "_")[:50]
        self.mark_frontier(url, f"skipped_{safe_reason}")

