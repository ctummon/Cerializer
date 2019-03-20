cmake_minimum_required(VERSION 3.7)

project (CerializerTests)

include_directories ("${PROJECT_SOURCE_DIR}")

set (CerializerTests_VERSION_MAJOR 0)
set (CerializerTests_VERSION_MINOR 1)
set (CMAKE_CXX_STANDARD 11)

add_subdirectory(Cerializer)
add_subdirectory(CerializerTests)


