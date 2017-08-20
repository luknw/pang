cmake_minimum_required(VERSION 3.7)
project(pang)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wpedantic")

set(SOURCE_FILES main.c)
add_executable(pang ${SOURCE_FILES})
target_link_libraries(pang m png)
