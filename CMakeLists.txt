cmake_minimum_required(VERSION 3.5)

project (CerializerTests)

set (CerializerTests_VERSION_MAJOR 0)
set (CerializerTests_VERSION_MINOR 1)
set (CMAKE_CXX_STANDARD 17)

add_compile_definitions(_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)

set (BUILD_CPP_REST   OFF CACHE BOOL "")
set (BUILD_QT         OFF CACHE BOOL "")
set (BUILD_RAPID_JSON ON  CACHE BOOL "")
set (BUILD_NLOHMANN   ON  CACHE BOOL "")
set (BUILD_SIMD_JSON  ON  CACHE BOOL "")

add_subdirectory(src/CerializerTests)


