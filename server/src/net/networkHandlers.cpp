#include "networkHandlers.h"

#include "../utils/SimpleJsonBuilder.h"
#include "../utils/expressionUtils.h"
#include "../utils/jsonUtils.h"
#include "../console/console.h"

#include <stdlib.h>
#include <random>
#include <unistd.h>
#include <thread>

void handleClientConnection(int socket)
{
    char buffer[1024] = {0};
    read(socket, buffer, 1024);

    int availabelThreads = std::thread::hardware_concurrency();
    if (availabelThreads == 0)
    {
        availabelThreads = 1;
    }

    availabelThreads = 4;

    std::string expression;
    std::string intervalStart;
    std::string intervalEnd;

    std::string bufferAsString(buffer);

    jsonGetProperty(bufferAsString, "expression", expression);
    jsonGetProperty(bufferAsString, "start", intervalStart);
    jsonGetProperty(bufferAsString, "end", intervalEnd);

    // Обработка ошибок в случае неправильной передачи данных
    // Nothing

    double intervalStartValue = std::stod(intervalStart);
    double intervalEndValue = std::stod(intervalEnd);

    std::cout << "Expression: " << expression << std::endl;
    std::cout << "Start: " << intervalStartValue << std::endl;
    std::cout << "End: " << intervalEndValue << std::endl;

    std::cout << "Computing function max value..." << std::endl;

    double yMin = 0;
    double yMax = computeMaxFuncValue(expression, intervalStartValue, intervalEndValue,
            (intervalEndValue - intervalStartValue) / POINTS_COUNT);

    ThreadData threadsData[availabelThreads];
    pthread_t threads[availabelThreads];

    pthread_mutex_t readyThreadsMutex;
    pthread_cond_t readyThreadsCondition;

    pthread_mutex_init(&readyThreadsMutex, NULL);
    pthread_cond_init(&readyThreadsCondition, NULL);

    int notStartedThreadsRemain = availabelThreads;

    for (int index = 0; index < availabelThreads; ++index)
    {
        threadsData[index].id = threads[index];
        threadsData->progress = 0;
        threadsData[index].expression = expression;
        threadsData[index].intervalLength = (intervalEndValue - intervalStartValue) / availabelThreads;
        threadsData[index].intervalStart = intervalStartValue + threadsData[index].intervalLength * index;
        threadsData[index].yMin = yMin;
        threadsData[index].yMax = yMax;
        threadsData[index].pointsOnInterval = POINTS_COUNT / availabelThreads;
        threadsData[index].readyThreadsMutex = &readyThreadsMutex;
        threadsData[index].readyThreadsCondition = &readyThreadsCondition;
        threadsData[index].notStartedThreadRemain = &notStartedThreadsRemain;
        threadsData[index].underPoints = 0;

        int error = pthread_create(threads + index, NULL, *computeIntegral, &threadsData[index]);
        if (error != 0)
        {
            throw std::runtime_error("Thread creation error");
        }
    }

    pthread_t consoleThread;
    ComputionDetailsArg diagnosticArgs;
    diagnosticArgs.notStartedThreadsRemain = &notStartedThreadsRemain;
    diagnosticArgs.readyThreadsMutex = &readyThreadsMutex;
    diagnosticArgs.readyCondition = &readyThreadsCondition;
    diagnosticArgs.threadsCount = availabelThreads;
    diagnosticArgs.threadsData = threadsData;
    pthread_create(&consoleThread, NULL, printComputionDetails, (void*) &diagnosticArgs);

    for (int index = 0; index < availabelThreads; ++index)
    {
        pthread_join(threads[index], NULL);
    }

    pthread_join(consoleThread, NULL);

    pthread_mutex_destroy(&readyThreadsMutex);
    pthread_cond_destroy(&readyThreadsCondition);

    int underPointsCount = 0;
    for (int index = 0; index < availabelThreads; ++index)
    {
        underPointsCount += threadsData[index].underPoints;
    }

    double result = (double) underPointsCount / POINTS_COUNT; 

    std::string response = SimpleJsonBuilder()
        .addProperty("result", std::string(std::to_string(result)))
        .toString();

    std::cout << "log: " << response << std::endl;

    write(socket, response.c_str(), response.size());
}

void* computeIntegral(void* args)
{
    ThreadData* threadData = (struct ThreadData*) args;

    std::string threadInfo;
    threadInfo.append("# Thread with ID " + std::to_string(threadData->id) + " started\n");
    threadInfo.append("  Interval start: " + std::to_string(threadData->intervalStart) +
            " Interval end: " + std::to_string(threadData->intervalStart + threadData->intervalLength) + "\n");
    threadInfo.append("  Interval points: " + std::to_string(threadData->pointsOnInterval) + "\n");

    std::cout << threadInfo;

    pthread_mutex_lock(threadData->readyThreadsMutex);
        // Декрементирует число неготовых потоков
        *(threadData->notStartedThreadRemain) -= 1;
        pthread_cond_signal(threadData->readyThreadsCondition);

    pthread_mutex_unlock(threadData->readyThreadsMutex);

    unsigned int seed = (unsigned int) threadData->id;
    double result;

    double pointX = threadData->intervalStart;
    double pointXEnd = threadData->intervalStart + threadData->intervalLength;
    double step = (pointXEnd - threadData->intervalStart) / (double) threadData->pointsOnInterval;

    double minY = threadData->yMin;
    double maxY = threadData->yMax;

    int pointUnderCount = 0;
    int completedPoints = 0;

    while (pointX < pointXEnd)
    {
        double pointY = minY + rand_r(&seed) % (long int) (maxY - minY);
        double pointXFunctionValue = computePostfixNotationExpression(threadData->expression, pointX);

        // std::cout << "Function result: " << pointXFunctionValue << " GeneratedPoint: " << pointY << std::endl;

        if (pointXFunctionValue > pointY)
        {
            (threadData->underPoints)++;
        }

        pointX += step;
        ++completedPoints;

        threadData->progress = (float) completedPoints / threadData->pointsOnInterval;
    }

    // Отадочный вывод
    // std::string points = "Points under: " + std::to_string(pointUnderCount) + " Points above: " + std::to_string(pointsAboveCount);
    // std::cout << points << std::endl;

    pthread_exit(nullptr);
}

