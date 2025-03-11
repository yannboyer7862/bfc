cmake_minimum_required(VERSION 3.25)

project(bfc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

option(USE_NATIVE_INSTRUCTIONS "Generate code for your specific CPU, may improve performance, at the cost of portability" OFF)

file(GLOB_RECURSE SOURCES src/**.cpp)
file(GLOB_RECURSE HEADERS include/**.hpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE include)

if (USE_NATIVE_INSTRUCTIONS AND NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()
