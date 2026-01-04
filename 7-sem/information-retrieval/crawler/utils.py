import hashlib
import re
from typing import Optional, Iterable
from urllib.parse import (
    urljoin,
    urlparse,
    urlunparse,
    parse_qsl,
    urlencode,
)


MEDIA_EXTENSIONS = (
    ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".svg", ".webp",
    ".mp4", ".avi", ".mov", ".wmv", ".flv", ".webm", ".mkv",
    ".mp3", ".wav", ".ogg", ".flac",
    ".pdf", ".doc", ".docx", ".xls", ".xlsx", ".ppt", ".pptx",
    ".zip", ".rar", ".7z",
)


_TRACKING_QUERY_RE = re.compile(
    r"^(utm_.*|gclid|fbclid|yclid|_hsenc|_hsmi|ref|refsrc|spm|mc_cid|mc_eid)$",
    re.IGNORECASE,
)


def is_media_file(url: str) -> bool:
    path = urlparse(url).path
    return path.lower().endswith(MEDIA_EXTENSIONS)


def get_domain(url: str) -> str:
    netloc = urlparse(url).netloc.lower()
    if netloc.startswith("www."):
        netloc = netloc[4:]
    if netloc.endswith(":80"):
        netloc = netloc[:-3]
    if netloc.endswith(":443"):
        netloc = netloc[:-4]
    return netloc


def get_source_name(url: str) -> str:
    """Source name for DB field. By default it's the domain without www."""
    try:
        return get_domain(url)
    except Exception:
        return "unknown"


def _strip_tracking_params(query: str) -> str:
    if not query:
        return ""
    pairs = []
    for k, v in parse_qsl(query, keep_blank_values=True):
        if _TRACKING_QUERY_RE.match(k):
            continue
        pairs.append((k, v))
    pairs.sort(key=lambda x: (x[0], x[1]))
    return urlencode(pairs, doseq=True)


def normalize_abs_url(url: str) -> Optional[str]:
    try:
        p = urlparse(url)
        if p.scheme not in ("http", "https"):
            return None

        scheme = "https"  # most modern sites redirect anyway; helps de-duplicate
        netloc = get_domain(url)

        query = _strip_tracking_params(p.query)
        # collapse multiple slashes in path (keep leading slash)
        path = re.sub(r"/{2,}", "/", p.path)

        normalized = urlunparse((scheme, netloc, path, "", query, ""))
        return normalized
    except Exception:
        return None


def normalize_url(base_url: str, link: str) -> Optional[str]:
    try:
        abs_url = urljoin(base_url, link)
        return normalize_abs_url(abs_url)
    except Exception:
        return None


def compile_regex_list(patterns: Optional[Iterable[str]]) -> list[re.Pattern]:
    compiled: list[re.Pattern] = []
    if not patterns:
        return compiled
    for p in patterns:
        compiled.append(re.compile(p))
    return compiled


def matches_any(patterns: list[re.Pattern], text: str) -> bool:
    return any(p.search(text) for p in patterns)


def hash_content(content: str) -> str:
    return hashlib.sha256(content.encode("utf-8", errors="ignore")).hexdigest()

