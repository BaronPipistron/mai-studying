-- Заполняем страны (из всех возможных источников)
INSERT INTO dim_countries (country_name)
SELECT DISTINCT customer_country FROM raw_data WHERE customer_country IS NOT NULL
UNION
SELECT DISTINCT seller_country FROM raw_data WHERE seller_country IS NOT NULL
UNION
SELECT DISTINCT store_country FROM raw_data WHERE store_country IS NOT NULL
UNION
SELECT DISTINCT supplier_country FROM raw_data WHERE supplier_country IS NOT NULL
ON CONFLICT (country_name) DO NOTHING;

-- Заполняем категории питомцев
INSERT INTO dim_pet_categories (category_name)
SELECT DISTINCT pet_category FROM raw_data WHERE pet_category IS NOT NULL
ON CONFLICT (category_name) DO NOTHING;

-- Заполняем типы питомцев
INSERT INTO dim_pet_types (type_name)
SELECT DISTINCT customer_pet_type FROM raw_data WHERE customer_pet_type IS NOT NULL
ON CONFLICT (type_name) DO NOTHING;

-- Заполняем цвета продуктов
INSERT INTO dim_colors (color_name)
SELECT DISTINCT product_color FROM raw_data WHERE product_color IS NOT NULL
ON CONFLICT (color_name) DO NOTHING;

-- Заполняем материалы
INSERT INTO dim_materials (name)
SELECT DISTINCT product_material FROM raw_data WHERE product_material IS NOT NULL
ON CONFLICT (name) DO NOTHING;

-- Заполняем категории продуктов
INSERT INTO dim_product_categories (category_name)
SELECT DISTINCT product_category FROM raw_data WHERE product_category IS NOT NULL
ON CONFLICT (category_name) DO NOTHING;

-- Заполняем бренды
INSERT INTO dim_brands (brand_name)
SELECT DISTINCT product_brand FROM raw_data WHERE product_brand IS NOT NULL
ON CONFLICT (brand_name) DO NOTHING;

-- Заполняем поставщиков
INSERT INTO dim_suppliers (
    name, contact, email, phone, address, city, country_id
)
SELECT DISTINCT
    r.supplier_name,
    r.supplier_contact,
    r.supplier_email,
    r.supplier_phone,
    r.supplier_address,
    r.supplier_city,
    c.country_id
FROM raw_data r
JOIN dim_countries c ON r.supplier_country = c.country_name
WHERE r.supplier_name IS NOT NULL
ON CONFLICT (name, email) DO NOTHING;

-- Заполняем магазины
INSERT INTO dim_stores (
    name, location, city, state, country_id, phone, email
)
SELECT DISTINCT
    r.store_name,
    r.store_location,
    r.store_city,
    r.store_state,
    c.country_id,
    r.store_phone,
    r.store_email
FROM raw_data r
JOIN dim_countries c ON r.store_country = c.country_name
WHERE r.store_name IS NOT NULL
ON CONFLICT (name, location) DO NOTHING;

-- Заполняем даты
INSERT INTO dim_date (
    date, day, month, year, quarter, day_of_week, is_weekend
)
SELECT DISTINCT
    r.sale_date,
    EXTRACT(DAY FROM r.sale_date),
    EXTRACT(MONTH FROM r.sale_date),
    EXTRACT(YEAR FROM r.sale_date),
    EXTRACT(QUARTER FROM r.sale_date),
    EXTRACT(DOW FROM r.sale_date),
    EXTRACT(DOW FROM r.sale_date) IN (0, 6)
FROM (
    SELECT DISTINCT sale_date FROM raw_data
    UNION
    SELECT DISTINCT product_release_date FROM raw_data WHERE product_release_date IS NOT NULL
    UNION
    SELECT DISTINCT product_expiry_date FROM raw_data WHERE product_expiry_date IS NOT NULL
) r
WHERE r.sale_date IS NOT NULL
ON CONFLICT (date) DO NOTHING;

-- Заполняем продукты
INSERT INTO dim_products (
    name, category_id, price, weight, color_id, size, brand_id,
    material_id, description, rating, reviews, release_date, expiry_date, supplier_id
)
SELECT DISTINCT
    r.product_name,
    pc.category_id,
    r.product_price,
    r.product_weight,
    col.color_id,
    r.product_size,
    b.brand_id,
    m.material_id,
    r.product_description,
    r.product_rating,
    r.product_reviews,
    r.product_release_date,
    r.product_expiry_date,
    s.supplier_id
FROM raw_data r
LEFT JOIN dim_product_categories pc ON r.product_category = pc.category_name
LEFT JOIN dim_colors col ON r.product_color = col.color_name
LEFT JOIN dim_brands b ON r.product_brand = b.brand_name
LEFT JOIN dim_materials m ON r.product_material = m.name
LEFT JOIN dim_suppliers s ON r.supplier_name = s.name AND r.supplier_email = s.email
WHERE r.product_name IS NOT NULL
ON CONFLICT (name, brand_id, supplier_id) DO NOTHING;

-- Заполняем продавцов
INSERT INTO dim_sellers (
    first_name, last_name, email, country_id, postal_code
)
SELECT DISTINCT
    r.seller_first_name,
    r.seller_last_name,
    r.seller_email,
    c.country_id,
    r.seller_postal_code
FROM raw_data r
JOIN dim_countries c ON r.seller_country = c.country_name
WHERE r.seller_first_name IS NOT NULL
ON CONFLICT (email) DO NOTHING;

-- Заполняем питомцев
INSERT INTO dim_pets (
    pet_type_id, pet_name, pet_breed, pet_category
)
SELECT DISTINCT
    pt.pet_type_id,
    r.customer_pet_name,
    r.customer_pet_breed,
    pc.pet_category_id
FROM raw_data r
JOIN dim_pet_types pt ON r.customer_pet_type = pt.type_name
JOIN dim_pet_categories pc ON r.pet_category = pc.category_name
WHERE r.customer_pet_name IS NOT NULL
ON CONFLICT (pet_name, pet_breed) DO NOTHING;

-- Заполняем клиентов
INSERT INTO dim_customers (
    first_name, last_name, age, email, country_id, postal_code, pet_id
)
SELECT DISTINCT
    r.customer_first_name,
    r.customer_last_name,
    r.customer_age,
    r.customer_email,
    c.country_id,
    r.customer_postal_code,
    p.pet_id
FROM raw_data r
JOIN dim_countries c ON r.customer_country = c.country_name
LEFT JOIN dim_pets p ON r.customer_pet_name = p.pet_name 
    AND (r.customer_pet_breed = p.pet_breed OR (r.customer_pet_breed IS NULL AND p.pet_breed IS NULL))
WHERE r.customer_first_name IS NOT NULL
ON CONFLICT (email) DO NOTHING;

-- Заполняем факты продаж
INSERT INTO fact_sales (
    customer_id, seller_id, product_id, store_id, date_id, quantity, total_price
)
SELECT
    c.customer_id,
    s.seller_id,
    p.product_id,
    st.store_id,
    d.date_id,
    r.sale_quantity,
    r.sale_total_price
FROM raw_data r
JOIN dim_customers c ON r.customer_email = c.email
JOIN dim_sellers s ON r.seller_email = s.email
JOIN dim_products p ON r.product_name = p.name 
    AND r.product_price = p.price
    AND (r.product_brand = (SELECT brand_name FROM dim_brands WHERE brand_id = p.brand_id) OR (r.product_brand IS NULL AND p.brand_id IS NULL))
JOIN dim_stores st ON r.store_name = st.name AND r.store_location = st.location
JOIN dim_date d ON r.sale_date = d.date
WHERE r.sale_quantity IS NOT NULL;