cmake_minimum_required(VERSION 3.10)
project(termable CXX)
set (CMAKE_CXX_STANDARD 17)

add_library(
    termable
    src/termable.cpp
)

target_include_directories(termable PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)