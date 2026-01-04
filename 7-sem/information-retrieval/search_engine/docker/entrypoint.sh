#!/usr/bin/env bash
set -euo pipefail

MONGO_URI="${MONGO_URI:-mongodb://mongo:27017}"
MONGO_DB="${MONGO_DB:-mai_ir_crawler}"
MONGO_COLLECTION="${MONGO_COLLECTION:-documents}"

INDEX_PATH="${INDEX_PATH:-/data/index.bin}"
ZIPF_PATH="${ZIPF_PATH:-/data/zipf.csv}"
PORT="${PORT:-8080}"

REINDEX_ON_START="${REINDEX_ON_START:-0}"   # 1 = always rebuild index on start
LIMIT="${LIMIT:-}"                          # optional: limit docs for test

mkdir -p /data

if [[ $# -gt 0 ]]; then
  exec "$@"
fi

if [[ ! -f "$INDEX_PATH" || "$REINDEX_ON_START" == "1" ]]; then
  echo "[entrypoint] Building index -> $INDEX_PATH"
  echo "[entrypoint] Mongo: $MONGO_URI / $MONGO_DB.$MONGO_COLLECTION"

  for i in $(seq 1 30); do
    set +e
    if [[ -n "$LIMIT" ]]; then
      /app/indexer --mongo-uri "$MONGO_URI" --db "$MONGO_DB" --collection "$MONGO_COLLECTION" \
        --index "$INDEX_PATH" --zipf "$ZIPF_PATH" --limit "$LIMIT"
    else
      /app/indexer --mongo-uri "$MONGO_URI" --db "$MONGO_DB" --collection "$MONGO_COLLECTION" \
        --index "$INDEX_PATH" --zipf "$ZIPF_PATH"
    fi
    rc=$?
    set -e

    if [[ $rc -eq 0 ]]; then
      echo "[entrypoint] Index built OK."
      break
    fi

    echo "[entrypoint] Indexer failed (attempt $i/30). Retrying in 2s..."
    sleep 2
  done
fi

echo "[entrypoint] Starting web server on :$PORT"
exec /app/search_server \
  --index "$INDEX_PATH" \
  --mongo-uri "$MONGO_URI" --db "$MONGO_DB" --collection "$MONGO_COLLECTION" \
  --port "$PORT"
