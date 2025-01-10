#include "jsonUtils.h"

int jsonGetProperty(const std::string& serializedJson, const std::string& property, std::string& result)
{
    int propertyPos = serializedJson.find(property);
    if (propertyPos == -1)
    {
        return -1;
    }

    int index = propertyPos + property.size() + 1;
    while (serializedJson[index++] != '"');

    while (serializedJson[index] != '"')
    {
        result.push_back(serializedJson[index++]);
    }

    return result.size();
}
