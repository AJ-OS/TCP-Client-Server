#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0601 // may change based on windows -v
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h> // use printf func
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "12345" // change to your port
#define DEFAULT_BUFLEN 512

#include "second.h"

// init server
void start_tcp(WSADATA wsaData, int &iResult)
{
    // Init Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed: %d.\n", iResult);
    }
    else
    {
        printf("Winsock dll Found!\n");
        printf("Status: %d.\n", iResult);
    }
}

SOCKET listen_connection(SOCKET &ClientSocket, SOCKET &ListenSocket, int &iResult)
{

    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed: %d.\n", iResult);
        WSACleanup();
    }
    else
    {
        printf("Local Found!\n");
    }

    // calls socket function listens for ip that matches returned value
    ListenSocket = INVALID_SOCKET;
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    // check for errors and ensure socket connection is valid
    if (ListenSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld.\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
    }
    else
    {
        printf("Socket Connection is Valid!\n");
    }

    // Setup the TCP listening socket

    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed with error: %d.\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
    }
    else
    {
        printf("Binding is ok.\n");
    }

    // listen on socket
    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("Listen failed with error: %ld.\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
    }
    else
    {
        printf("Waiting for Connection...\n");
    }

    freeaddrinfo(result);

    // Accept a client socket
    ClientSocket = INVALID_SOCKET;

    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET)
    {
        printf("accept failed: %d.\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
    }
    else
    {
        printf("Connection Accepted!\n");
    }
}

// accept client and receive text/files
SOCKET accept_client(SOCKET &ClientSocket, SOCKET &ListenSocket, int &iResult)
{
    bool found = true;
    while (found)
    {
        char recvbuf[DEFAULT_BUFLEN];
        int iSendResult;

        iResult = recv(ClientSocket, recvbuf, sizeof(recvbuf), 0);

        if (iResult > 0)
        {
            printf("Data Accepted!\n");
            printf("Bytes Received: %d\n", iResult);
            recvbuf[iResult] = '\0';                       // Null-terminate
            printf("Received from client: %s\n", recvbuf);

            FILE *fp;
            const char *filename = "recv.txt";
            fp = fopen(filename, "a");
            if (fp == NULL)
            {
                perror("Error opening file");
                return ListenSocket;
            }
            fprintf(fp, "%s", recvbuf); // Write received data to the file
            fclose(fp);
        }
        else if (iResult == 0)
        {
            printf("Sender Disconnected.\n");
            break;
        }
        else
        {
            printf("Error Receiving Data!\n");
            // Handle error 
            break;
        }
    }
    printf("Waiting for Data...\n");
    return ListenSocket;
}
