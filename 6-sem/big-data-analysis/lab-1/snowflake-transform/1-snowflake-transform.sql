CREATE TABLE fact_sales (
    sale_id SERIAL PRIMARY KEY,
    customer_id INT REFERENCES dim_customers(customer_id),
    seller_id INT REFERENCES dim_sellers(seller_id),
    product_id INT REFERENCES dim_products(product_id),
    store_id INT REFERENCES dim_stores(store_id),
    date_id INT REFERENCES dim_date(date_id),
    quantity INT,
    total_price NUMERIC(10, 2)
);

CREATE TABLE dim_customers (
    customer_id SERIAL PRIMARY KEY,
    first_name TEXT,
    last_name TEXT,
    age INT,
    email TEXT,
    country_id INT REFERENCES dim_countries(country_id),
    postal_code TEXT,
    pet_id INT REFERENCES dim_pets(pet_id)
);

CREATE TABLE dim_pets (
    pet_id SERIAL PRIMARY KEY,
    pet_type_id INT REFERENCES dim_pet_types(pet_type_id),
    pet_name TEXT,
    pet_breed TEXT,
    pet_category INT REFERENCES dim_pet_categories(pet_category_id)
);

CREATE TABLE dim_pet_categories (
    pet_category_id SERIAL PRIMARY KEY,
    category_name TEXT NOT NULL UNIQUE
);

CREATE TABLE dim_pet_types (
    pet_type_id SERIAL PRIMARY KEY,
    type_name TEXT NOT NULL UNIQUE
);

CREATE TABLE dim_sellers (
    seller_id SERIAL PRIMARY KEY,
    first_name TEXT,
    last_name TEXT,
    email TEXT,
    country_id INT REFERENCES dim_countries(country_id),
    postal_code TEXT
);

CREATE TABLE dim_products (
    product_id SERIAL PRIMARY KEY,
    name TEXT,
    category_id INT REFERENCES dim_product_categories(category_id),
    price NUMERIC(10, 2),
    weight NUMERIC(10, 2),
    color_id INT REFERENCES dim_colors(color_id),
    size TEXT,
    brand_id INT REFERENCES dim_brands(brand_id),
    material_id INT REFERENCES dim_materials(material_id),
    description TEXT,
    rating NUMERIC(3, 1),
    reviews INT,
    release_date DATE,
    expiry_date DATE,
    supplier_id INT REFERENCES dim_suppliers(supplier_id)
);

CREATE TABLE dim_colors (
    color_id SERIAL PRIMARY KEY,
    color_name TEXT NOT NULL UNIQUE
);

CREATE TABLE dim_materials (
    material_id SERIAL PRIMARY KEY,
    name TEXT NOT NULL UNIQUE
);

CREATE TABLE dim_product_categories (
    category_id SERIAL PRIMARY KEY,
    category_name TEXT NOT NULL UNIQUE
);

CREATE TABLE dim_brands (
    brand_id SERIAL PRIMARY KEY,
    brand_name TEXT NOT NULL UNIQUE
);

CREATE TABLE dim_stores (
    store_id SERIAL PRIMARY KEY,
    name TEXT,
    location TEXT,
    city TEXT,
    state TEXT,
    country_id INT REFERENCES dim_countries(country_id),
    phone TEXT,
    email TEXT
);

CREATE TABLE dim_suppliers (
    supplier_id SERIAL PRIMARY KEY,
    name TEXT,
    contact TEXT,
    email TEXT,
    phone TEXT,
    address TEXT,
    city TEXT,
    country_id INT REFERENCES dim_countries(country_id)
);

CREATE TABLE dim_countries (
    country_id SERIAL PRIMARY KEY,
    country_name TEXT NOT NULL UNIQUE
);

CREATE TABLE dim_date (
    date_id SERIAL PRIMARY KEY,
    date DATE,
    day INT,
    month INT,
    year INT,
    quarter INT,
    day_of_week INT,
    is_weekend BOOLEAN
);
