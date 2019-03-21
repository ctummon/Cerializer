set(SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/catch.hpp)

if (${BUILD_CPP_REST})
list(APPEND SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/CppRestJsonTests.cpp)
endif (${BUILD_CPP_REST})

if (${BUILD_QT})
list(APPEND SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/QJsonTests.cpp)
endif(${BUILD_QT})

if (${BUILD_RAPID_JSON})
list(APPEND SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/RapidJsonTests.cpp)
endif(${BUILD_RAPID_JSON})

if (${BUILD_NLOHMANN})
list(APPEND SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/NlohmannJsonTests.cpp)
endif(${BUILD_NLOHMANN})

add_executable(CerializerTests ${SourceFiles})

target_include_directories(
    CerializerTests PUBLIC
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/thirdparty/rapidjson/include"
    "${PROJECT_SOURCE_DIR}/thirdparty/json/include"
)

if (${BUILD_CPP_REST})
find_package(cpprestsdk REQUIRED)
target_link_libraries(CerializerTests PRIVATE cpprestsdk::cpprest)
endif (${BUILD_CPP_REST})

if (${BUILD_QT})
find_package(Qt5Core CONFIG REQUIRED)
target_link_libraries(CerializerTests PRIVATE Qt5::Core)
endif(${BUILD_QT})



target_link_libraries(CerializerTests INTERFACE Cerializer)
