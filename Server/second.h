#ifndef SECOND_H
#define SECOND_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "12345" // Define DEFAULT_PORT here

void start_tcp(WSADATA wsaData, int &iResult);
SOCKET listen_connection(SOCKET &ClientSocket, SOCKET &ListenSocket, int &iResult);
SOCKET accept_client(SOCKET &ClientSocket, SOCKET &ListenSocket, int &iResult);

#endif
