#include "threadUtils.h"
#include "expressionUtils.h"

#include <stdlib.h>
#include <random>

void* computeIntegral(void* args)
{
    ThreadData* threadData = (struct ThreadData*) args;

    std::string threadInfo;
    threadInfo.append("# Thread with ID " + std::to_string(threadData->id) + " started\n");
    threadInfo.append("Interval start: " + std::to_string(threadData->intervalStart) +
            " Interval end: " + std::to_string(threadData->intervalStart + threadData->intervalLength));

    std::cout << threadInfo << std::endl;

    unsigned int seed = (unsigned int) threadData->id;
    double result;

    double pointX = threadData->intervalStart;
    double pointXEnd = threadData->intervalStart + threadData->intervalLength;
    double step = (pointXEnd - threadData->intervalStart) / (double) POINTS_COUNT;

    double minY = threadData->yMin;
    double maxY = threadData->yMax;

    int pointUnderCount = 0;
    int pointsAboveCount = 0;

    while (pointX < pointXEnd)
    {
        double pointY = minY + rand_r(&seed) % (long int) (maxY - minY);
        double pointXFunctionValue = computePostfixNotationExpression(threadData->expression, pointX);

        // std::cout << "Function result: " << pointXFunctionValue << " GeneratedPoint: " << pointY << std::endl;

        if (pointXFunctionValue > pointY)
        {
            ++pointUnderCount;
        }
        else
        {
            ++pointsAboveCount;
        }

        pointX += step;

        threadData->progress = pointX / pointXEnd * 100;
    }

    std::string points = "Points under: " + std::to_string(pointUnderCount) + " Points above: " + std::to_string(pointsAboveCount);
    std::cout << points << std::endl;

    threadData->underPoints = pointUnderCount;
    threadData->allIntervalPoints = pointUnderCount + pointsAboveCount;

    std::string message = "Thread with ID " + std::to_string(threadData->id) + " is ready.";

    std::cout << message << std::endl;

    pthread_exit(nullptr);
}
