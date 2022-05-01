cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_executable(
    test
    test.cpp
)

target_link_libraries(
    test 
    termable
  )

add_executable(
    buffer
    buffer.cpp
)

target_link_libraries(
    buffer 
    termable
  )


add_executable(
    double_buffer
    double_buffer.cpp
)

target_link_libraries(
    double_buffer 
    termable
  )

add_executable(
    mover
    mover.cpp
)

target_link_libraries(
    mover 
    termable
)

add_executable(
    menu_test
    menu_test.cpp
)

target_link_libraries(
    menu_test
    termable
    termable_ui
)

add_executable(
    kb_test
    kb_test.cpp
)

target_link_libraries(
    kb_test
    termable
)
            
add_executable(
    border_test
    border_test.cpp
)
target_link_libraries(
    border_test
    termable
    termable_ui
)

add_executable(
    table_test
    table_test.cpp
)

target_link_libraries(
    table_test
    termable
    termable_ui
)