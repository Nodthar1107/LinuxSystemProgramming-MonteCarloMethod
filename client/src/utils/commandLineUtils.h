#include <iostream>

int getValueFromArgs(int argc, char* argv[], const std::string& flag, const std::string& shortenFlag, std::string& dest);

int includesFlag(int argc, char* argv[], const std::string& flag, const std::string& shortenFlag);