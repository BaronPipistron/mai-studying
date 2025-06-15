import os
import json
import pandas as pd
import time
import socket

from typing import List
from kafka import KafkaProducer


def wait_for_kafka(host: str = 'localhost', port: int = 9092, timeout: int = 60) -> None:
    """Ожидает доступности Kafka сервера в течение заданного времени."""
    start_time = time.time()
    while time.time() - start_time < timeout:
        try:
            with socket.create_connection((host, port), timeout=1):
                print(f"[✓] Kafka доступен по адресу {host}:{port}")
                return
        except OSError:
            print(f"[...] Waiting Kafka ({host}:{port})...")
            time.sleep(2)
    raise Exception(f"[ERROR] Kafka don't response {timeout} s")


def find_csv_files(data_dir: str) -> List[str]:
    """Находит все CSV файлы с 'mock_data' в названии в указанной директории."""
    return sorted([
        f for f in os.listdir(data_dir)
        if f.lower().endswith('.csv') and 'mock_data' in f.lower()
    ])


def send_data_to_kafka(producer: KafkaProducer, topic: str, data_dir: str, files: List[str]) -> None:
    """Отправляет данные из CSV файлов в Kafka."""
    for filename in files:
        full_path = os.path.join(data_dir, filename)
        try:
            df = pd.read_csv(full_path)
            print(f"[+] Sending data from file: {filename}")

            for index, row in df.iterrows():
                json_data = row.to_dict()
                producer.send(topic, json_data)
                print(f"   → String {index + 1} sent to Kafka successfully")
                time.sleep(0.01)

        except Exception as e:
            print(f"[ERROR] Error while processing file {filename}: {e}")


def main():
    KAFKA_HOST = 'localhost'
    KAFKA_PORT = 9092
    DATA_DIR = 'исходные данные'
    TOPIC_NAME = 'mock-topic'

    wait_for_kafka(KAFKA_HOST, KAFKA_PORT)

    producer = KafkaProducer(
        bootstrap_servers=f'{KAFKA_HOST}:{KAFKA_PORT}',
        value_serializer=lambda v: json.dumps(v, ensure_ascii=False).encode('utf-8')
    )

    csv_files = find_csv_files(DATA_DIR)
    print(f"[INFO] Files found: {len(csv_files)}")
    print(csv_files)

    send_data_to_kafka(producer, TOPIC_NAME, DATA_DIR, csv_files)

    producer.flush()
    print("All data successfully sent to Kafka")


if __name__ == '__main__':
    main()

