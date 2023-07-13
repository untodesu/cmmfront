cmake_minimum_required(VERSION 3.10)
project(pfd LANGUAGES CXX VERSION 1.1.0)
add_library(pfd INTERFACE)
target_include_directories(pfd INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")
