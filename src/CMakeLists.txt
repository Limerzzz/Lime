file(GLOB lime_lib_source *.cpp)
set(lime_lib_include ${PROJECT_SOURCE_DIR}/src/include)

add_library(lime_shared SHARED ${lime_lib_source})
target_include_directories(lime_shared PUBLIC ${lime_lib_include})
