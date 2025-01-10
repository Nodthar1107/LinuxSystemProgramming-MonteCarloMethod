#include "socket.h"

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

sockaddr_in createAddressDescriptor(int port)
{
    struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

    return addr;   
}

int createServerSocket(sockaddr_in& addr, int connectionsCount)
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == 0)
	{
		throw std::runtime_error("Socket create error");
	}

	int addrlen = sizeof(addr);

	if(bind(serverSocket, (struct sockaddr *)&addr, sizeof(addr)) > 0)
	{
		throw std::runtime_error("Socket bind error");
	}

	if (listen(serverSocket, connectionsCount) < 0)
	{
		throw std::runtime_error("Listen error");
	}

    return serverSocket;
}
