cmake_minimum_required(VERSION 3.10)
project(rapidjson LANGUAGES CXX VERSION 1.1.0)
add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")
