#### 1. Запуск Docker инфраструктуры:

```bash
docker-compose up -d
```

Поднимется Kafka, Flink, PostgreSQL и pgAdmin.

---

#### 2. Выполнить `inti.sql` для вашей базы данных

---

#### 3. Собрать Flink Job:

```bash
cd flink-snowflake-job
mvn clean package
```

Файл появится в `target/*.jar`

---

#### 4. Отправка данных в Kafka:

```bash
python producer.py
```

* Проверяется Kafka
* Создаётся топик `mock-topic`
* Файлы из `./исходные данные` улетают в Kafka

---

#### 5. Деплой Flink Job через Web UI:

* Перейти: [http://localhost:8081](http://localhost:8081)
* Нажать **Submit New Job**
* Загрузить `.jar` из `target/`
* Entry class:

```
FlinkKafkaToPostgres
```