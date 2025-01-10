#include <string>

class SimpleJsonBuilder
{
private:
    std::string buffer;
    int propertiesCount = 0;

public:
    SimpleJsonBuilder();
    SimpleJsonBuilder addProperty(const std::string& property, const std::string& value);
    std::string toString();
};
