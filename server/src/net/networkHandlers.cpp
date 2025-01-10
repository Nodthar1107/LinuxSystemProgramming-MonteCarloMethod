#include "networkHandlers.h"

#include "../utils/SimpleJsonBuilder.h"
#include "../utils/expressionUtils.h"
#include "../utils/jsonUtils.h"
#include "../utils/threadUtils.h"

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

    double yMin = 0;
    double yMax = computeMaxFuncValue(expression, intervalStartValue, intervalEndValue,
            (intervalEndValue - intervalStartValue) / POINTS_COUNT);

    ThreadData threadsData[availabelThreads];
    pthread_t threads[availabelThreads];

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

        int error = pthread_create(threads + index, NULL, *computeIntegral, threadsData + index);
        if (error != 0)
        {
            throw std::runtime_error("Thread creation error");
        }
    }

    for (int index = 0; index < availabelThreads; ++index)
    {
        pthread_join(threads[index], nullptr);
    }

    int underPointsCount = 0;
    int allPointsCount = 0;
    for (int index = 0; index < availabelThreads; ++index)
    {
        underPointsCount += threadsData[index].underPoints;
    }

    double result = (double) underPointsCount / POINTS_COUNT; 

    std::cout << "Result: " << result << std::endl;

    std::string response = SimpleJsonBuilder()
        .addProperty("result", std::string(std::to_string(result)))
        .toString();

    std::cout << response << std::endl;

    write(socket, response.c_str(), response.size());
}
