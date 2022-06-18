file(GLOB lime_lib_source *.cc)
set(lime_lib_include ${PROJECT_SOURCE_DIR}/src/include)

find_package(Threads)
add_library(lime_shared SHARED ${lime_lib_source})
target_include_directories(lime_shared PUBLIC ${lime_lib_include})
target_link_libraries(lime_shared ${CMAKE_THREAD_LIBS_INIT})
