#include <string>

struct ThreadData
{
    unsigned long int id;
    std::string expression;
    double intervalStart;
    double intervalLength;

    double yMin;
    double yMax;
    double pointsOnInterval;

    int progress;
    int underPoints;
};

struct ComputionResult
{
    int id;
    double result;
};


const int POINTS_COUNT = 10000000;

void* computeIntegral(void* args);
