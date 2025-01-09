#include <iostream>

class InvalidCharacterException : public std::exception
{
private:
    std::string message;
    int position;

public:
    InvalidCharacterException(std::string message, int position);

    std::string getMessage() const;

    int getPosition() const;
};
