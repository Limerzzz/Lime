
add_executable(client client.cpp)
target_include_directories(client PRIVATE ${PROJECT_SOURCE_DIR}/src/include)
target_link_libraries(client lime_shared)

add_executable(server server.cpp)
target_include_directories(server PRIVATE ${PROJECT_SOURCE_DIR}/src/include)
target_link_libraries(server lime_shared)

