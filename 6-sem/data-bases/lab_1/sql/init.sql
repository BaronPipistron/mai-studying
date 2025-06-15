-- Создаем тип статуса платежа
CREATE TYPE payment_status AS ENUM ('PENDING', 'COMPLETED', 'FAILED', 'REFUNDED');

-- Создаем таблицу платежей
CREATE TABLE payment
(
    payment_id        SERIAL PRIMARY KEY,
    booking_id        INTEGER        NOT NULL,
    customer_id       INTEGER        NOT NULL,
    amount            NUMERIC(10, 2) NOT NULL,
    currency          CHAR(3)        NOT NULL DEFAULT 'USD',
    payment_time      TIMESTAMPTZ    NOT NULL,
    payment_method    VARCHAR(50),
    status            payment_status NOT NULL,
    transaction_id    VARCHAR(100),
    confirmation_code VARCHAR(100),
    refunded_amount   NUMERIC(10, 2)          DEFAULT 0,
    refund_time       TIMESTAMPTZ,
    error_code        VARCHAR(50),
    error_message     TEXT,
    notes             TEXT,
    created_at        TIMESTAMPTZ    NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at        TIMESTAMPTZ    NOT NULL DEFAULT CURRENT_TIMESTAMP
);

-- Устанавливаем расширения
CREATE EXTENSION IF NOT EXISTS pg_trgm;
CREATE EXTENSION IF NOT EXISTS pg_bigm;
CREATE EXTENSION IF NOT EXISTS pgcrypto;