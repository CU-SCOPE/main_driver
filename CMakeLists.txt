cmake_minimum_required(VERSION 2.8.9)
project(flood LANGUAGES C CXX)

include_directories("${PROJECT_SOURCE_DIR}/src")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
add_executable(scope src/main.cpp src/scope.cpp)
target_compile_options(scope PUBLIC -O3 -pthread)
