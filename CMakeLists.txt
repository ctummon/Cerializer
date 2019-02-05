cmake_minimum_required(VERSION 3.7)

project (CerializerTests)

include_directories ("${PROJECT_SOURCE_DIR}")

set (CerializerTests_VERSION_MAJOR 0)
set (CerializerTests_VERSION_MINOR 1)

if (${BUILD_CPP_REST})
add_compile_definitions(BUILD_CPPREST_OBJ)
#find_package(cpprestsdk REQUIRED NAMES cpprestsdk cpprest)
#find_package(ZLIB REQUIRED)
endif (${BUILD_CPP_REST})

add_subdirectory(Cerializer)
add_subdirectory(CerializerTests)

#(${BUILD_CPP_REST})
#target_link_libraries(main PRIVATE cpprestsdk::cpprest)
#target_link_libraries(main PRIVATE ZLIB::ZLIB)
#(${BUILD_CPP_REST})
