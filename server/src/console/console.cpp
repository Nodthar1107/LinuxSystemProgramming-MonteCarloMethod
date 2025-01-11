#include "console.h"

#include <iostream>

const std::string computionProcessHeader = "** Computing integral **";

void* printComputionDetails(void* args)
{
    ComputionDetailsArg* details = (ComputionDetailsArg*) args;

    // Проверка, что все потоки запустились
    pthread_mutex_lock(details->readyThreadsMutex);
    while (*details->notStartedThreadsRemain > 0)
    {
        pthread_cond_wait(details->readyCondition, details->readyThreadsMutex);
    }
    pthread_mutex_unlock(details->readyThreadsMutex);

    std::cout <<std::endl << computionProcessHeader << std::endl << std::endl;

    ThreadData* threadsData = details->threadsData;
    bool readyState[details->threadsCount] = {false};

    while (true)
    {
        for (int index = 0; index < details->threadsCount; ++index)
        {
            int loaded = (int) (threadsData[index].progress * 10) % 11;
            int notLoaded = 10 - loaded;

            std::cout << "Thread " << index + 1
                << "[" << std::string(loaded, '#') << std::string(notLoaded, '.') << "]";

            readyState[index] = loaded == 10;

            if (readyState[index])
            {
                std::cout << " Thread with ID " << threadsData[index].id << " is ready";
            }

            std::cout << std::endl;
        }

        int ready = 0;
        for (int index = 0; index < details->threadsCount; ++index)
        {
            if (readyState[index])
            {
                ++ready;
            }
        }

        if (ready == details->threadsCount)
        {
            break;
        }

        std::cout << "\r" << "\033[" + std::to_string(details->threadsCount) + "A";
    }

    std::cout << std::endl;

    pthread_exit(nullptr);
}


