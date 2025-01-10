#include "expressionUtils.h"

#include <vector>
#include <cmath>

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

int toDigit(char c)
{
    return c - 48;
}

double computePostfixNotationExpression(const std::string& expression, double argument)
{
    std::vector<double> operands;
    std::string variable;

    int operand = 0;
    bool ignoreOperandPushing = false;

    for (int charIndex = 0; charIndex < expression.size(); ++charIndex)
    {
        if (isDigit(expression.at(charIndex)))
        {
            operand = operand * 10 + toDigit(expression.at(charIndex));
        }
        else if (isVariableSymbol(expression.at(charIndex)))
        {
            variable.push_back(expression.at(charIndex));
        }
        else if (isOperation(expression.at(charIndex)))
        {
            double operand2 = operands.back();
            operands.pop_back();
            double operand1 = operands.back();
            operands.pop_back();

            switch (expression.at(charIndex))
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
        else if (expression.at(charIndex) == ' ')
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

double computeMaxFuncValue(const std::string& expression, double start, double end, double step)
{
    // Предполагаем, что вся функция лежит выше оси X
    double maxValue = 0;
    while (start < end)
    {
        double currentValue = computePostfixNotationExpression(expression, start);
        if (currentValue > maxValue)
        {
            maxValue = currentValue;
        }

        start += step;
    }

    return maxValue;
}
