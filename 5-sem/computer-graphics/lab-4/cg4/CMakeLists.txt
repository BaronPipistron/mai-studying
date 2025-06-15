cmake_minimum_required(VERSION 3.25)
project(cg4)

set(CMAKE_CXX_STANDARD 23)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)

set(glfw3_STATIC_LIBRARIES TRUE)
set(glfw3_DIR C:/glfw3/lib/cmake/glfw3)

include_directories(C:/SFML/include)
include_directories(C:/glfw3/include)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(FreeGLUT REQUIRED)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenGL::GL
        GLEW::GLEW
        FreeGLUT::freeglut
        glfw
        sfml-system sfml-window sfml-graphics sfml-audio)