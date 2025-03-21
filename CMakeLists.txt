cmake_minimum_required(VERSION 3.27)

project(bfc)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED YES)

file(GLOB_RECURSE SOURCES src/**.c)
file(GLOB_RECURSE HEADERS include/**.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE include)
