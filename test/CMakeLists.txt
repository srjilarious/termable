cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 17)

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
