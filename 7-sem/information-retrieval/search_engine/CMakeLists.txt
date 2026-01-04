cmake_minimum_required(VERSION 3.16)
project(ir_search_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies (via vcpkg manifest mode)
find_package(bsoncxx CONFIG REQUIRED)
find_package(mongocxx CONFIG REQUIRED)
find_package(httplib CONFIG REQUIRED)
find_package(Threads REQUIRED)

# Pick static libs if vcpkg provides them, otherwise use shared
set(IR_BSONCXX_TARGET $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>)
set(IR_MONGOCXX_TARGET $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_shared>)

add_library(ir_core
    src/html_strip.cpp
    src/tokenizer.cpp
    src/stemmer.cpp
    src/index.cpp
    src/boolean_query.cpp
    src/io.cpp
    src/zipf.cpp
    src/mongo_reader.cpp
)

target_include_directories(ir_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(ir_core PRIVATE ${IR_MONGOCXX_TARGET} ${IR_BSONCXX_TARGET})

add_executable(indexer src/main_indexer.cpp)
target_link_libraries(indexer PRIVATE ir_core)

add_executable(search_cli src/main_cli.cpp)
target_link_libraries(search_cli PRIVATE ir_core)

add_executable(search_server src/main_server.cpp)
target_link_libraries(search_server PRIVATE ir_core httplib::httplib Threads::Threads)
