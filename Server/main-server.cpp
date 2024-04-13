#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0601 // may change based on windows -v
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h> // sleep func
#include <stdio.h>   // use printf func
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "12345" // change to your port
#define DEFAULT_BUFLEN 512

#include "second.h"

// run with admin power in terminal/power shell with following commands (change cd to your directory)
// --------------------
// cd "file location"
// gcc main-server.cpp & second.cpp -o server.exe -lws2_32 (change to the location of your files)
// g++ main-server.cpp second.cpp -o main-server.exe -lws2_32
// .\server.exe
// --------------------

// run .bat file
// Example: C:\Users\aj-os\Desktop\file-tcp-server-cpp\server.bat (change to the location of your .bat file)

int main()
{
    WSADATA wsaData;
    int iResult;
    SOCKET ListenSocket;
    SOCKET ClientSocket;

    do
    {
        start_tcp(wsaData, iResult);
        ListenSocket = listen_connection(ClientSocket, ListenSocket, iResult);

        ClientSocket = accept_client(ClientSocket, ListenSocket, iResult);
        printf("Client Connected.\n");
        if (ClientSocket == INVALID_SOCKET)
        {
            printf("Accept Failed: %d.\n", WSAGetLastError());
            closesocket(ClientSocket);
            break;
        }

        closesocket(ClientSocket);
        WSACleanup();

    } while (true);

    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}
