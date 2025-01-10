#include <string>

struct ThreadData
{
    unsigned long int id;
    std::string expression;
    double intervalStart;
    double intervalLength;

    double yMin;
    double yMax;

    int progress;
    int underPoints;
    int allIntervalPoints;
};

struct ComputionResult
{
    int id;
    double result;
};


const int POINTS_COUNT = 10000;

void* computeIntegral(void* args);
