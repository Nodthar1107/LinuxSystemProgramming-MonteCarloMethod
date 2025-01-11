#ifndef NETWORK_HANDLERS
#define NETWORK_HANDLERS

#include <string>
#include <thread>

void handleClientConnection(int socket);

bool isClientConnectionAlive(int socket);

struct ThreadData
{
    unsigned long int id;
    std::string expression;
    double intervalStart;
    double intervalLength;

    double yMin;
    double yMax;
    int pointsOnInterval;

    float progress;
    int underPoints;

    int* notStartedThreadRemain;
    pthread_mutex_t* readyThreadsMutex;
    pthread_cond_t* readyThreadsCondition;
};

const int POINTS_COUNT = 10000000;

void* computeIntegral(void* args);

#endif
