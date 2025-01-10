#include <netinet/in.h>

int createServerSocket(sockaddr_in& addr, int connectionsCount);

sockaddr_in createAddressDescriptor(int port);
