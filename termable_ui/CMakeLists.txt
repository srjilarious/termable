cmake_minimum_required(VERSION 3.10)
project(termable CXX)
set (CMAKE_CXX_STANDARD 17)

add_library(
    termable_ui
    src/termable_ui.cpp
)

target_include_directories(termable_ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(
    termable_ui 
    termable
)