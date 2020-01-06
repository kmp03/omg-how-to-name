cmake_minimum_required(VERSION 3.5)

project(hello_world LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES main.cpp implementation.cpp)
set(HEADERS vint.h)

add_executable(hello_world ${SOURCES} ${HEADERS})
