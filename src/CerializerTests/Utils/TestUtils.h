#include <fstream>
#include <string>

namespace Cerializer {
std::string
getTestJson()
{
    std::string content;
    std::ifstream file(
      "C:/dev/Cerializer/CerializerTests/TestData/TestObject.json",
      std::ios::binary);
    if (file.is_open()) {
        content.assign((std::istreambuf_iterator<char>(file)),
          (std::istreambuf_iterator<char>()));
    }
    return content;
}
} // namespace Cerializer
