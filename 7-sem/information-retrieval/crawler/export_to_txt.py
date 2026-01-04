import os
import yaml
import re
from pymongo import MongoClient
from bs4 import BeautifulSoup


OUTPUT_DIR = "corpus_files"


def load_config(config_path: str):
    with open(config_path, 'r', encoding='utf-8') as f:
        return yaml.safe_load(f)


def clean_text(text):
    if not text:
        return ""
    text = text.replace('\xa0', ' ').replace('\r', ' ')
    text = re.sub(r'\s+', ' ', text)
    return text.strip()


def extract_article_data(html_content):
    if not html_content:
        return None

    soup = BeautifulSoup(html_content, "lxml")

    h1_tag = soup.find("h1")
    title = clean_text(h1_tag.get_text()) if h1_tag else "Без заголовка"

    meta_desc = soup.find("meta", attrs={"name": "description"})
    description = clean_text(meta_desc["content"]) if meta_desc else "Описание отсутствует"

    tags_to_remove = [
        "script", "style", "header", "footer", "nav", "aside",
        "form", "iframe", "noscript", "meta", "link", "svg", "button"
    ]
    for tag in soup(tags_to_remove):
        tag.decompose()

    junk_selectors = [
        "comments", "comments-wrapper", "sidebar",
        re.compile(r'.*comments.*'),  # Любые блоки комментариев
        re.compile(r'.*read-more.*'),  # Блоки "читайте также"
        re.compile(r'.*sidebar.*'),  # Сайдбары
        re.compile(r'.*banner.*'),  # Реклама
        re.compile(r'.*social.*'),  # Кнопки соцсетей
        re.compile(r'.*promo.*'),  # Промо блоки
        "post-metadata",  # Метаданные автора/даты (Playground)
        "mezzanine",  # Верхняя плашка Playground
        "article-content-footer"  # Футер статьи Playground
    ]

    for selector in junk_selectors:
        if isinstance(selector, str):
            for t in soup.find_all(attrs={"id": selector}): t.decompose()
            for t in soup.find_all(class_=selector): t.decompose()
        else:
            for t in soup.find_all(class_=selector): t.decompose()

    content_soup = None

    potential_containers = [
        soup.find(id="material_content"),
        soup.find(class_="article-content"),
        soup.find("article", class_=re.compile(r"prose")),
        soup.find("div", class_=re.compile(r"material-content_\d+")),
        soup.find("div", class_=re.compile(r"TextContent_text")),
        soup.find("article")  # Generic
    ]

    for container in potential_containers:
        if container:
            content_soup = container
            break

    if not content_soup:
        content_soup = soup.body if soup.body else soup

    full_text = clean_text(content_soup.get_text(separator=' '))

    return {
        "title": title,
        "description": description,
        "text": full_text
    }


def main():
    if not os.path.exists("config.yaml"):
        print("Ошибка: файл config.yaml не найден.")
        return

    config = load_config("config.yaml")

    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)

    try:
        client = MongoClient(config['db']['connection_string'])
        db = client[config['db']['database_name']]
        collection = db[config['db']['doc_collection']]
    except Exception as e:
        print(f"Ошибка подключения к БД: {e}")
        return

    print("Начинаю обработку документов...")
    count = 0

    cursor = collection.find({}, {"_id": 1, "raw_html": 1})

    for doc in cursor:
        raw_html = doc.get("raw_html")

        if raw_html:
            data = extract_article_data(raw_html)

            if data and data["text"]:
                doc_id = str(doc["_id"])
                filename = os.path.join(OUTPUT_DIR, f"{doc_id}.txt")

                try:
                    with open(filename, "w", encoding="utf-8") as f:
                        f.write(f"{data['title']}\n")
                        f.write(f"{data['description']}\n")
                        f.write(data['text'])
                    count += 1
                except Exception as e:
                    print(f"Ошибка записи {filename}: {e}")

        if count % 100 == 0 and count > 0:
            print(f"Обработано: {count}...")

    print(f"\nГотово! Сохранено {count} файлов в '{OUTPUT_DIR}'.")


if __name__ == "__main__":
    main()

    