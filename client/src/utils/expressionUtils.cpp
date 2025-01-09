#include <cmath>
#include <vector>

#include "../exceptions/InvalidCharacterException.h"
#include "expressionUtils.h"

bool isDigit(char c)
{
    return c - 48 >= 0 && c - 48 <= 9;
}

bool isOperation(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool isVariableSymbol(char c)
{
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

int getPriority(char c)
{
    switch (c)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    default:
        return -1;
    }
}

int toDigit(char c)
{
    return c - 48;
}

std::string toPostfixNotation(const std::string& source)
{
    std::string result;
    std::vector<char> operations;

    bool mustBeVariable = true;
    bool mustBeNumber = true;
    bool mustBeOperation = false;

    for (int charIndex = 0; charIndex < source.size(); ++charIndex)
    {
        if (isVariableSymbol(source[charIndex]))
        {
            if (!mustBeVariable)
            {
                throw InvalidCharacterException("Invalid variable character", charIndex);
            }

            mustBeNumber = false;
            mustBeOperation = false;

            result.push_back(source[charIndex]);
        }
        else if (isDigit(source[charIndex]))
        {
            if (!mustBeNumber)
            {
                throw InvalidCharacterException("Invalid digit character", charIndex);
            }

            mustBeOperation = false;
            mustBeVariable = false;

            result.push_back(source[charIndex]);
        }
        else if (source[charIndex] == ' ')
        {
            if (result.back() != ' ')
            {
                result.push_back(' ');
            }

            if (mustBeNumber || mustBeVariable)
            {
                mustBeOperation = true;

                mustBeVariable = false;
                mustBeNumber = false;
            }
            else
            {
                mustBeVariable = true;
                mustBeNumber = true;

                mustBeOperation = false;
            }
        }
        else if (isOperation(source[charIndex]))
        {
            if (!mustBeOperation)
            {
                throw InvalidCharacterException("Invalid operation character", charIndex);
            }

            if (operations.empty())
            {
                operations.push_back(source[charIndex]);

                continue;
            }

            int currentOperationPriority = getPriority(source[charIndex]);
            int topOperationPriority = -1;
            while (!operations.empty() && ((topOperationPriority = getPriority(operations.back())) > currentOperationPriority))
            {
                if (result.back() != ' ')
                {
                    result.push_back(' ');
                }
                result.push_back(operations.back());

                operations.pop_back();
            }

            operations.push_back(source[charIndex]);
        }
        else
        {
            throw InvalidCharacterException("Illegal character", charIndex);
        }
    }

    if (mustBeOperation)
    {
        throw InvalidCharacterException("Expression must be ended with argument or number. Not operation", source.size() - 1);
    }

    while (!operations.empty())
    {
        char operation = operations.back();
        operations.pop_back();

        result.push_back(' ');
        result.push_back(operation);
    }

    return result;
}

double computePostfixNotationExpression(const std::string& expression, double argument)
{
    std::vector<double> operands;
    std::string variable;

    int operand = 0;
    bool ignoreOperandPushing = false;

    for (int charIndex = 0; charIndex < expression.size(); ++charIndex)
    {
        if (isDigit(expression[charIndex]))
        {
            operand = operand * 10 + toDigit(expression[charIndex]);
        }
        else if (isVariableSymbol(expression[charIndex]))
        {
            variable.push_back(expression[charIndex]);
        }
        else if (isOperation(expression[charIndex]))
        {
            double operand2 = operands.back();
            operands.pop_back();
            double operand1 = operands.back();
            operands.pop_back();

            switch (expression[charIndex])
            {
            case '+':
                operands.push_back(operand1 + operand2);
                break;
            case '-':
                operands.push_back(operand1 - operand2);
                break;
            case '*':
                operands.push_back(operand1 * operand2);
                break;
            case '/':
                operands.push_back(operand1 / operand2);
                break;
            case '^':
                operands.push_back(pow(operand1, operand2));
                break;
            }

            ignoreOperandPushing = true;
        }
        else if (expression[charIndex] == ' ')
        {
            if (ignoreOperandPushing)
            {
                ignoreOperandPushing = false;

                continue;
            }

            if (variable.empty())
            {
                operands.push_back(operand);
                operand = 0;
            }
            else
            {
                operands.push_back(argument);
                variable.clear();
            }
        }
    }

    return operands.back();
}