#include "InvalidCharacterException.h"

InvalidCharacterException::InvalidCharacterException(std::string message, int position) : message(message), position(position)
{
    // Nothing
}

std::string InvalidCharacterException::getMessage() const
{
    return message;
}

int InvalidCharacterException::getPosition() const
{
    return position;
}