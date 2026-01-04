from typing import Dict, Any

import yaml
import asyncio
import sys
from database import Database
from crawler import Crawler


def load_config(config_path: str) -> Dict[str, Any]:
    print(f"Загрузка конфигурации из: {config_path}")
    with open(config_path, 'r', encoding='utf-8') as f:
        config = yaml.safe_load(f)
    return config


async def main(config_path: str):
    config = load_config(config_path)

    try:
        db = Database(config)
        print(f"Подключен к MongoDB, база: {config['db']['database_name']}")
    except Exception as e:
        print(f"Ошибка подключения к MongoDB: {e}")
        return

    crawler = Crawler(config, db)

    print("Поисковый робот запущен. Нажмите Ctrl+C для остановки.")
    try:
        await crawler.run()
    except KeyboardInterrupt:
        print("\nПолучен сигнал KeyboardInterrupt. Завершаю работу...")
    finally:
        await crawler.client.aclose()
        db.client.close()
        print("Клиенты HTTP и MongoDB закрыты. Выход.")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Ошибка: Неверное количество аргументов.")
        print("Использование: python main.py config.yaml")
        sys.exit(1)

    config_file_path = sys.argv[1]
    asyncio.run(main(config_file_path))

    