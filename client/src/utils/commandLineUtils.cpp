#include "commandLineUtils.h"
#include <cstring>

int includesFlag(int argc, char* argv[], const std::string& flag, const std::string& shortenFlag)
{
    for (int argIndex = 1; argIndex < argc; argIndex++)
    {
        if (strcmp(argv[argIndex], flag.c_str()) == 0 || strcmp(argv[argIndex], shortenFlag.c_str()) == 0)
        {
            return argIndex;
        }
    }

    return -1;
}

int getValueFromArgs(int argc, char* argv[], const std::string& flag, const std::string& shortenFlag, std::string& dest)
{
    int pos = includesFlag(argc, argv, flag, shortenFlag);
    if (pos != -1 && pos + 1 <= argc)
    {
        std::string valueCandidate(argv[pos + 1]);
        if (valueCandidate[0] != '-')
        {
            dest = valueCandidate;

            return valueCandidate.size();
        }
    }

    return -1;
}
