# mai-information-retrieval

## IR Search Engine (C++)

Учебный проект поисковой системы:
- индексация документов из MongoDB
- токенизация + стемминг
- булев индекс + булев поиск (AND/OR/NOT + скобки)
- 2 интерфейса: CLI и веб (HTML форма)

### Синтаксис запросов
Поддерживаются:
- `AND`, `OR`, `NOT` (регистр не важен)
- `(` и `)`
- Неявный `AND` между соседними термами: `rust mongodb` == `rust AND mongodb`

## CLI поиск
Из корня проекта
```bash
cd search_engine
echo "mongodb AND (c++ OR rust) NOT windows" | docker compose run --rm -T search /app/search_cli --index /data/index.bin
```

## Веб поиск
Из корня проекта
```bash
cd search_engine
docker compose up -d
```

Откройте:
- http://localhost:8080

## Crawler
Также был реализован робот для обкачки документов с `habr.com` и `ru.stackoverflow.com`. Обкачивает документы с указанных доменов и сохраняет их в монго. По скачанным документам в дальнейшем производится поиск

```bash
cd crawler

# Запускаем mongo
docker compose up -d

# Запускаем crawler'а
python main.py config.yaml
```