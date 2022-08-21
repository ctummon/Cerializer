set(Headers ${CMAKE_CURRENT_SOURCE_DIR}/Serializable.h)

list(APPEND Headers ${CMAKE_CURRENT_SOURCE_DIR}/CerialUtils.h)

set(SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

if (${BUILD_CPP_REST})
list(APPEND Headers 
    ${CMAKE_CURRENT_SOURCE_DIR}/CppRestJsonObj.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/CppRestJsonConverter.h)

list(APPEND SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/CppRestJsonTests.cpp)
endif (${BUILD_CPP_REST})

if (${BUILD_QT})
list(APPEND Headers 
    ${CMAKE_CURRENT_SOURCE_DIR}/QJsonObj.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/QJsonConverter.h)

list(APPEND SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/QJsonTests.cpp)
endif(${BUILD_QT})

if (${BUILD_RAPID_JSON})
list(APPEND Headers 
    ${CMAKE_CURRENT_SOURCE_DIR}/RapidJsonObj.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/RapidJsonConverter.h)

list(APPEND SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/RapidJsonTests.cpp)
endif(${BUILD_RAPID_JSON})

if (${BUILD_NLOHMANN})
list(APPEND Headers 
    ${CMAKE_CURRENT_SOURCE_DIR}/NlohmannJsonObj.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/NlohmannJsonConverter.h)

list(APPEND SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/NlohmannJsonTests.cpp)
endif(${BUILD_NLOHMANN})

if (${BUILD_SIMD_JSON})
list(APPEND Headers 
    ${CMAKE_CURRENT_SOURCE_DIR}/SimdJsonObj.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/SimdJsonConverter.h)

list(APPEND SourceFiles 
    ${CMAKE_CURRENT_SOURCE_DIR}/SimdJsonTests.cpp
    ${PROJECT_SOURCE_DIR}/thirdparty/simdjson/singleheader/simdjson.cpp)
endif(${BUILD_SIMD_JSON})

add_executable(CerializerTests ${SourceFiles})

target_include_directories(
    CerializerTests PUBLIC
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/thirdparty/"
    "${PROJECT_SOURCE_DIR}/thirdparty/rapidjson/include"
    "${PROJECT_SOURCE_DIR}/thirdparty/nlohmann/json/include"
    "${PROJECT_SOURCE_DIR}/thirdparty/simdjson/singleheader/"
)

if (${BUILD_CPP_REST})
find_package(cpprestsdk REQUIRED NAMES cpprestsdk cpprest)
target_link_libraries(CerializerTests PRIVATE cpprestsdk::cpprest)
endif (${BUILD_CPP_REST})

if (${BUILD_QT})
find_package(Qt5Core CONFIG REQUIRED)
target_link_libraries(CerializerTests PRIVATE Qt5::Core)
endif(${BUILD_QT})



target_link_libraries(CerializerTests INTERFACE Cerializer)
