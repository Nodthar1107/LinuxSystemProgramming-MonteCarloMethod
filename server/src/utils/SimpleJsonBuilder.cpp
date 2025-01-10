#include "SimpleJsonBuilder.h"
#include <iostream>

SimpleJsonBuilder::SimpleJsonBuilder()
{
    buffer.push_back('{');
}

SimpleJsonBuilder SimpleJsonBuilder::addProperty(const std::string& property, const std::string& value)
{
    buffer.append(property + ": \"" + value + "\",");
    propertiesCount++;

    return *this;
}

std::string SimpleJsonBuilder::toString()
{
    if (propertiesCount == 0) {
        throw std::runtime_error("Json object is empty. It must be constructed with properties");
    }

    buffer.pop_back();
    buffer.push_back('}');

    return buffer;
}
