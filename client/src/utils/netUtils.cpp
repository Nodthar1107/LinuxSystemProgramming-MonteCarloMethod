#include "netUtils.h"

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int connectToServer(std::string ip, int port)
{
    struct sockaddr_in addr; // структура с адресом
    struct hostent* hostinfo;
    int sock = socket(AF_INET, SOCK_STREAM, 0); // создание TCP-сокета

    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    // Указываем параметры сервера
    addr.sin_family = AF_INET; // домены Internet
    addr.sin_port = htons(port); // или любой другой порт...
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) // установка соединения с сервером
    {
        std::cerr << "Connection error" << std::endl;
        exit(0);

        close(sock);
    }

    return sock;
}
