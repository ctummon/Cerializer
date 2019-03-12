add_library(Cerializer INTERFACE)

set(Headers ${CMAKE_CURRENT_SOURCE_DIR}/Serializable.h)

list(APPEND Headers ${CMAKE_CURRENT_SOURCE_DIR}/CerialUtils.h)

if (${BUILD_CPP_REST})
list(APPEND Headers 
    ${CMAKE_CURRENT_SOURCE_DIR}/CppRestJsonObj.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/CppRestJsonConverter.h)
endif (${BUILD_CPP_REST})

if (${BUILD_QT})
list(APPEND Headers 
    ${CMAKE_CURRENT_SOURCE_DIR}/QJsonObj.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/QJsonConverter.h)
endif (${BUILD_QT})

if (${BUILD_RAPID_JSON})
list(APPEND Headers 
    ${CMAKE_CURRENT_SOURCE_DIR}/RapidJsonObj.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/RapidJsonConverter.h)
endif (${BUILD_RAPID_JSON})

if (${BUILD_NLOHMANN})
list(APPEND Headers 
    ${CMAKE_CURRENT_SOURCE_DIR}/NlohmannJsonObj.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/NlohmannJsonConverter.h)
endif (${BUILD_NLOHMANN})

target_include_directories(Cerializer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(Cerializer INTERFACE ${Headers}
)

add_custom_target(Cerializer_ SOURCES ${Headers})