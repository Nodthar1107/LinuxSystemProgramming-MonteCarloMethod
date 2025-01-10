#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

const int SERVER_PORT = 8080;

int main()
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == 0)
	{
		// Throw error
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
		throw std::runtime_error("Clietn socket error");
	}

	char buffer[1024] = {0};
	read(clientSocket, buffer, 1024);
	std::cout << buffer << std::endl;

	std::string respose = "Hello from server";
	write(clientSocket, respose.c_str(), respose.size());

	return 0;
}
