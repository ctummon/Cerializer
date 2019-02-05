set(SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/catch.hpp)

if (${BUILD_CPP_REST})
list(APPEND SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/CppRestJsonTests.cpp)
endif (${BUILD_CPP_REST})

add_executable(CerializerTests ${SourceFiles})

target_link_libraries(CerializerTests INTERFACE Cerializer)