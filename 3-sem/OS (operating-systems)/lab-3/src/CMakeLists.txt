cmake_minimum_required(VERSION 3.10)
project(Lab3)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

add_executable(_run_exe run.cpp)
add_executable(_calculator_exe calculator.cpp)