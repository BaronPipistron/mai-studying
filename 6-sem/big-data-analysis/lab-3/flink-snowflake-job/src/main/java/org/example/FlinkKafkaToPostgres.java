package org.example;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.flink.api.common.serialization.SimpleStringSchema;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.streaming.connectors.kafka.FlinkKafkaConsumer;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.util.Properties;

public class FlinkKafkaToPostgres {

    public static void main(String[] args) throws Exception {

        final StreamExecutionEnvironment env = StreamExecutionEnvironment.getExecutionEnvironment();

        Properties properties = new Properties();
        properties.setProperty("bootstrap.servers", "kafka:9092");
        properties.setProperty("group.id", "flink-group");
        properties.setProperty("auto.offset.reset", "earliest");

        FlinkKafkaConsumer<String> kafkaConsumer = new FlinkKafkaConsumer<>(
                "mock-topic",
                new SimpleStringSchema(),
                properties
        );

        env.addSource(kafkaConsumer)
            .map(value -> {
                ObjectMapper mapper = new ObjectMapper();
                JsonNode json = mapper.readTree(value);

                try (Connection conn = DriverManager.getConnection(
                        "jdbc:postgresql://localhost:5432/star_schema",
                        "postgres", "postgres"
                )) {
                    conn.setAutoCommit(false);

                    // === CUSTOMERS ===
                    PreparedStatement customerStmt = conn.prepareStatement(
                        "INSERT INTO customers (id, first_name, last_name, age, email, country, postal_code, pet_type, pet_name, pet_breed) " +
                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?) " +
                        "ON CONFLICT (id) DO NOTHING"
                    );
                    customerStmt.setString(1, json.get("sale_customer_id").asText());
                    customerStmt.setString(2, json.get("customer_first_name").asText());
                    customerStmt.setString(3, json.get("customer_last_name").asText());
                    customerStmt.setInt(4, json.get("customer_age").asInt());
                    customerStmt.setString(5, json.get("customer_email").asText());
                    customerStmt.setString(6, json.get("customer_country").asText());
                    customerStmt.setString(7, json.get("customer_postal_code").asText());
                    customerStmt.setString(8, json.get("customer_pet_type").asText());
                    customerStmt.setString(9, json.get("customer_pet_name").asText());
                    customerStmt.setString(10, json.get("customer_pet_breed").asText());
                    customerStmt.executeUpdate();

                    // === SELLERS ===
                    PreparedStatement sellerStmt = conn.prepareStatement(
                        "INSERT INTO sellers (id, first_name, last_name, email, country, postal_code) " +
                        "VALUES (?, ?, ?, ?, ?, ?) " +
                        "ON CONFLICT (id) DO NOTHING"
                    );
                    sellerStmt.setString(1, json.get("sale_seller_id").asText());
                    sellerStmt.setString(2, json.get("seller_first_name").asText());
                    sellerStmt.setString(3, json.get("seller_last_name").asText());
                    sellerStmt.setString(4, json.get("seller_email").asText());
                    sellerStmt.setString(5, json.get("seller_country").asText());
                    sellerStmt.setString(6, json.get("seller_postal_code").asText());
                    sellerStmt.executeUpdate();

                    // === PRODUCTS ===
                    PreparedStatement productStmt = conn.prepareStatement(
                        "INSERT INTO products (id, name, category, price, quantity, weight, color, size, brand, material, description, rating, reviews, release_date, expiry_date) " +
                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) " +
                        "ON CONFLICT (id) DO NOTHING"
                    );
                    productStmt.setString(1, json.get("sale_product_id").asText());
                    productStmt.setString(2, json.get("product_name").asText());
                    productStmt.setString(3, json.get("product_category").asText());
                    productStmt.setDouble(4, json.get("product_price").asDouble());
                    productStmt.setInt(5, json.get("product_quantity").asInt());
                    productStmt.setDouble(6, json.get("product_weight").asDouble());
                    productStmt.setString(7, json.get("product_color").asText());
                    productStmt.setString(8, json.get("product_size").asText());
                    productStmt.setString(9, json.get("product_brand").asText());
                    productStmt.setString(10, json.get("product_material").asText());
                    productStmt.setString(11, json.get("product_description").asText());
                    productStmt.setDouble(12, json.get("product_rating").asDouble());
                    productStmt.setInt(13, json.get("product_reviews").asInt());
                    productStmt.setString(14, json.get("product_release_date").asText());
                    productStmt.setString(15, json.get("product_expiry_date").asText());
                    productStmt.executeUpdate();

                    // === STORES ===
                    PreparedStatement storeStmt = conn.prepareStatement(
                        "INSERT INTO stores (name, location, city, state, country, phone, email) " +
                        "VALUES (?, ?, ?, ?, ?, ?, ?) " +
                        "ON CONFLICT (name) DO NOTHING"
                    );
                    storeStmt.setString(1, json.get("store_name").asText());
                    storeStmt.setString(2, json.get("store_location").asText());
                    storeStmt.setString(3, json.get("store_city").asText());
                    storeStmt.setString(4, json.get("store_state").asText());
                    storeStmt.setString(5, json.get("store_country").asText());
                    storeStmt.setString(6, json.get("store_phone").asText());
                    storeStmt.setString(7, json.get("store_email").asText());
                    storeStmt.executeUpdate();

                    // === SUPPLIERS ===
                    PreparedStatement supplierStmt = conn.prepareStatement(
                        "INSERT INTO suppliers (name, contact, email, phone, address, city, country) " +
                        "VALUES (?, ?, ?, ?, ?, ?, ?) " +
                        "ON CONFLICT (name) DO NOTHING"
                    );
                    supplierStmt.setString(1, json.get("supplier_name").asText());
                    supplierStmt.setString(2, json.get("supplier_contact").asText());
                    supplierStmt.setString(3, json.get("supplier_email").asText());
                    supplierStmt.setString(4, json.get("supplier_phone").asText());
                    supplierStmt.setString(5, json.get("supplier_address").asText());
                    supplierStmt.setString(6, json.get("supplier_city").asText());
                    supplierStmt.setString(7, json.get("supplier_country").asText());
                    supplierStmt.executeUpdate();

                    // === SALES (fact) ===
                    PreparedStatement saleStmt = conn.prepareStatement(
                        "INSERT INTO sales (id, customer_id, seller_id, product_id, quantity, total_price, sale_date, store_name) " +
                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?) " +
                        "ON CONFLICT (id) DO NOTHING"
                    );
                    saleStmt.setString(1, json.get("id").asText());
                    saleStmt.setString(2, json.get("sale_customer_id").asText());
                    saleStmt.setString(3, json.get("sale_seller_id").asText());
                    saleStmt.setString(4, json.get("sale_product_id").asText());
                    saleStmt.setInt(5, json.get("sale_quantity").asInt());
                    saleStmt.setDouble(6, json.get("sale_total_price").asDouble());
                    saleStmt.setString(7, json.get("sale_date").asText());
                    saleStmt.setString(8, json.get("store_name").asText());
                    saleStmt.executeUpdate();

                    conn.commit();

                } catch (Exception e) {
                    System.err.println("[ERROR] " + e.getMessage());
                }

                return value;
            .print()
            });

        env.execute("Flink Kafka → PostgreSQL (Snowflake Model)");
    }
}
