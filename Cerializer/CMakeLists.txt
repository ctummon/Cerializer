add_library(Cerializer INTERFACE)

set(Headers ${CMAKE_CURRENT_SOURCE_DIR}/Serializable.h)

if (${BUILD_CPP_REST})
list(APPEND Headers 
    ${CMAKE_CURRENT_SOURCE_DIR}/CppRestJsonObj.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/CppRestJsonConverter.h)
endif (${BUILD_CPP_REST})

target_include_directories(Cerializer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(Cerializer INTERFACE ${Headers}
)

add_custom_target(Cerializer_ SOURCES ${Headers})