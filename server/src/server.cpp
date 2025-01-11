#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>
#include <stdlib.h>
#include <signal.h> 

#include "utils/SimpleJsonBuilder.h"
#include "utils/commandLineUtils.h"
#include "utils/expressionUtils.h"
#include "utils/jsonUtils.h"
#include "net/socket.h"

#include "net/networkHandlers.h"

const int DEFAULT_PORT = 8080;
const std::string MANUAL = "** Server of integral computing **\n\n"
	"This utility program works as server for integral computing. it can work independently or in conjunction with other "
	"servers on the local network.\n\n"
	"Options       Long flags         Meaning\n"
	"-t            --threads-count    Set number of thread for parallel integral computing. By default, the number "
	"of threads that can be created is taken from the system.\n"
	"-p            --points           Sets the number of points on the x-axis. By default it use 100 points";

int serverSocket;

void handleInterruptSignal(int signal);

int main(int argc, char* argv[])
{
	if (includesFlag(argc, argv, "--help", "-h") > -1)
	{
		std::cout << MANUAL << std::endl;

		return 0;
	}

	signal(SIGINT, handleInterruptSignal);

	int error = 0;
	std::string threadsCountAsString;
	int availabelThreads = std::thread::hardware_concurrency();
	if (availabelThreads == 0)
	{
		availabelThreads = 1;
	}

	int threadsCount = availabelThreads;
	error = getValueFromArgs(argc, argv, "--threads-count", "-t", threadsCountAsString);
	if (error != -1)
	{
		threadsCount = std::stoi(threadsCountAsString);
	}

	std::string pointsAsString;
	int points = POINTS_COUNT;
	error = getValueFromArgs(argc, argv, "-points", "-p", pointsAsString);
	if (error != -1)
	{
		points = std::stoi(threadsCountAsString);
	}

	sockaddr_in addr = createAddressDescriptor(DEFAULT_PORT);

	serverSocket = createServerSocket(addr, 1);

	int addrlen = sizeof(addr);
	std::cout << "Server listening on port " << DEFAULT_PORT << "..." << std::endl;

	while (true)
	{
		int clientSocket = accept(serverSocket, (struct sockaddr *)&addr, (socklen_t*)&addrlen);
		if (clientSocket < 0)
		{
			throw std::runtime_error("Client socket error");
		}

		try
		{
			handleClientConnection(clientSocket, threadsCount, points);
		}
		catch(const std::exception& e)
		{
			std::cerr << "Connection refuesd" << std::endl;
		}

		close(clientSocket);		
	}

	return 0;
}

void handleInterruptSignal(int signal)
{
	std::cout << "Error: Interrupt" << std::endl;

	close(serverSocket);
	exit(signal);
}
