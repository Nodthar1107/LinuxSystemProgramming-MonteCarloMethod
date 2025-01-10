#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>
#include <stdlib.h>

#include "utils/threadUtils.h"
#include "utils/SimpleJsonBuilder.h"
#include "utils/jsonUtils.h"

#include "utils/expressionUtils.h"

const int SERVER_PORT = 8080;

int main()
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == 0)
	{
		throw std::runtime_error("Socket create error");
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int addrlen = sizeof(addr);

	if(bind(serverSocket, (struct sockaddr *)&addr, sizeof(addr)) > 0)
	{
		throw std::runtime_error("Socket bind error");
	}

	if (listen(serverSocket, 1) < 0)
	{
		throw std::runtime_error("Listen error");
	}

	std::cout << "Server listening on port " << SERVER_PORT << "..." << std::endl;

	int clientSocket = accept(serverSocket, (struct sockaddr *)&addr, (socklen_t*)&addrlen);

	if (clientSocket < 0)
	{
		throw std::runtime_error("Client socket error");
	}

	char buffer[1024] = {0};
	read(clientSocket, buffer, 1024);
	std::cout << "Data from client:" << std::endl << buffer << std::endl;

	int availabelThreads = std::thread::hardware_concurrency();
	if (availabelThreads == 0)
	{
		availabelThreads = 1;
	}

	availabelThreads = 3;

	std::cout << "Availabel threads count: " << availabelThreads << std::endl;

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
		allPointsCount += threadsData[index].allIntervalPoints;
	}

	double result = (double) underPointsCount / allPointsCount; 

	std::cout << "Result: " << result << std::endl;

	std::string response = SimpleJsonBuilder()
		.addProperty("result", std::string(std::to_string(result)))
		.toString();

	std::cout << response << std::endl;

	write(clientSocket, response.c_str(), response.size());

	return 0;
}
