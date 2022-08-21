#include <string_view>

// Make changes in TestDataFilePath.in.h as it will overwrite .h file.
namespace TestData {
constexpr auto TestDataFilePath =
  "${CMAKE_CURRENT_SOURCE_DIR}/TestData/TestObject.json";
};
