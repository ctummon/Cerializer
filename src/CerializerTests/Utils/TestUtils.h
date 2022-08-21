#include "TestDataFilePath.h"

#include <fstream>
#include <string>

namespace Cerializer {
template<typename StringType>
StringType
getTestJson()
{
    StringType content;
    std::ifstream file(TestData::TestDataFilePath, std::ios::binary);
    if (file.is_open()) {
        content.assign((std::istreambuf_iterator<StringType::value_type>(file)),
          (std::istreambuf_iterator<StringType::value_type>()));
    }
    return content;
}
} // namespace Cerializer
