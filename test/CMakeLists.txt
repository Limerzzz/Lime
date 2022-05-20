
add_executable(client client.cpp util.cpp)
target_include_directories(client PRIVATE ${PROJECT_BINARY_DIR})


add_executable(server server.cpp util.cpp)
target_include_directories(server PRIVATE ${PROJECT_BINARY_DIR})

