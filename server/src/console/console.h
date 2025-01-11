#ifndef CONSOLE
#define CONSOLE

#include "../net/networkHandlers.h"

#include <pthread.h>

struct ComputionDetailsArg
{
    ThreadData* threadsData;
    int threadsCount;

    int* notStartedThreadsRemain;
    pthread_mutex_t* readyThreadsMutex;
    pthread_cond_t* readyCondition;
};

void* printComputionDetails(void* args);

#endif
