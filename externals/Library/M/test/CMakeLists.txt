include_directories(${M_SOURCE_DIR}/include)
add_executable(teststring main)
target_link_libraries(teststring M crypto)
