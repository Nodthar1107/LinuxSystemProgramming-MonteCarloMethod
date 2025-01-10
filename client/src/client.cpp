#include <iostream>
#include <string.h>
#include <unistd.h>
#include "exceptions/InvalidCharacterException.h"
#include "utils/expressionUtils.h"
#include "utils/commandLineUtils.h"
#include "utils/netUtils.h"
#include "utils/SimpleJsonBuilder.h"

const std::string HELP_FLAG = "--help";
const std::string HELP_FLAG_SHORTEN = "-h";
const std::string EXPRESSION_FLAG = "--expression";
const std::string EXPRESSION_FLAG_SHORTEN = "-E";
const std::string INTERVAL_START_FLAG = "--interval-start";
const std::string INTERVAL_START_FLAG_SHORTEN = "-s";
const std::string INTERVAL_END_FLAG = "--interval-end";
const std::string INTERVAL_END_FLAG_SHORTEN = "-e";

const std::string MANUAL = "** Network calculation of the integral on the interval **\n\n"
    "This utility program calculate integral of the given function on provided interval. Program uses Monte-Carlo algorithm\n"
    "so the result may not be as accurate. Limitations: provided function must be placed higher than Ox.\n\n"
    "Required flags Long flags          Meaning\n"
    "-E             --expression        Provide function will be integrated\n"
    "-s             --interval-start    Provide start point of integration. Type double. It will be included\n"
    "-e             --interval-end      Provide end point of integration. Type dщuble. It won't be included\n\n"
    "Option         Long option         Meaning\n"
    "-h             --help              Show manual\n\n"
    "Usage example:\n"
    "-e \"x ^ 2 * 24 + 2\" -s -10 -e 10";

const std::string EXPRESSION_MISSING_ERROR = "Arguments error: Expression must be provided via flag --expression in brackets\n";
const std::string EXPRESSION_PARSE_ERROR = "Expression parse error: Incorrect expression format\nExpression example: \"x ^ 2 + 24\"";
const std::string INTERVAL_START_MISSING_ERROR = "Arguments error: Interval start is missing. It must be provided via --interval-start as double value";
const std::string INTERVAL_END_MISSING_ERROR = "Arguments error: Interval end is missing. It must be provided via --interval-end as double value";
const std::string INTERVAL_START_FORMAT_ERROR = "Double format error: Interval start value must be double";
const std::string INTERVAL_END_FORMAT_ERROR = "Double format error: Interval end value must be double";
const std::string INTERVAL_RANGE_ERROR = "Arguments error: Interval start must be less then interval end";

double castToDoubleDangerours(const std::string& value, const std::string error)
{
    double dValue = 0;

    try
    {
        dValue = std::stod(value);
    }
    catch(const std::exception& e)
    {
        std::cout << INTERVAL_START_FORMAT_ERROR << std::endl;
        exit(0);
    }

    return dValue;
}

int main(int argc, char* argv[])
{
    if (includesFlag(argc, argv, HELP_FLAG, HELP_FLAG_SHORTEN) > -1)
    {
        std::cout << MANUAL << std::endl;

        return 0;
    }

    int error = 0;

    std::string expression;
    error = getValueFromArgs(argc, argv, EXPRESSION_FLAG, EXPRESSION_FLAG_SHORTEN, expression);
    if (error == -1)
    {
        std::cerr << EXPRESSION_MISSING_ERROR;

        return 0;
    }

    std::cout << "Expression: " << expression << std::endl;

    std::string intervalStart;
    error = getValueFromArgs(argc, argv, INTERVAL_START_FLAG, INTERVAL_START_FLAG_SHORTEN, intervalStart);
    if (error == -1)
    {
        std::cerr << INTERVAL_START_MISSING_ERROR << std::endl;

        return 0;
    }

    std::string intervalEnd;
    error = getValueFromArgs(argc, argv, INTERVAL_END_FLAG, INTERVAL_END_FLAG_SHORTEN, intervalEnd);
    if (error == -1)
    {
        std::cerr << INTERVAL_END_MISSING_ERROR << std::endl;

        return 0;
    }

    double leftValue = castToDoubleDangerours(intervalStart, INTERVAL_START_FORMAT_ERROR);
    double rightValue = castToDoubleDangerours(intervalEnd, INTERVAL_END_FORMAT_ERROR);
    if (leftValue > rightValue)
    {
        std::cerr<< INTERVAL_RANGE_ERROR << std::endl;

        return 0;
    }

    std::string postfixNotationExpression;
    try
    {
        postfixNotationExpression = toPostfixNotation(expression);
    }
    catch(const InvalidCharacterException e)
    {
        std::cerr << EXPRESSION_PARSE_ERROR << std::endl;
        std::cerr << e.getMessage() << std::endl;
        std::cerr << "Error position: " << e.getPosition() << std::endl;
    }

    int socket = connectToServer("127.0.0.1", 8080);

    std::cout << "Connected to server" << std::endl; 

    SimpleJsonBuilder builder;
    std::string data = builder
        .addProperty("expression", postfixNotationExpression)
        .addProperty("start", intervalStart)
        .addProperty("end", intervalEnd)
        .toString();

    write(socket, data.c_str(), data.size());

    char buff[1024];
    read(socket, buff, 1024);

    std::string result(buff);

    std::cout << result << std::endl;

    return 0;
}
