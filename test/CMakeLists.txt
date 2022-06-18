
add_executable(client client.cc)
target_include_directories(client PRIVATE ${PROJECT_SOURCE_DIR}/src/include)
target_link_libraries(client lime_shared)

add_executable(server server.cc)
target_include_directories(server PRIVATE ${PROJECT_SOURCE_DIR}/src/include)
target_link_libraries(server lime_shared)

add_executable(thread_test thread_test.cc)
target_include_directories(thread_test PRIVATE ${PROJECT_SOURCE_DIR}/src/include)
target_link_libraries(thread_test lime_shared)

