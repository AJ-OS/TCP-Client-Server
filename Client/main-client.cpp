#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h> // delete if no need for time/sleep();

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "123"
#define DEFAULT_SERVER "123.123.1.123" // Change this to the server IP address 
#define DEFAULT_BUFLEN 512
#define SOCKET_READ_TIMEOUT_SEC 5

// cd "file"
// g++ main-client.cpp -o client.exe -lws2_32
// .\client.exe

// run .bat file
// Example: C:\Users\aj-os\Desktop\TCP-Client\client.bat

int main()
{
    SOCKET ConnectSocket = INVALID_SOCKET;
    WSADATA wsaData;
    int iResult;
    bool found = true;

    // search for server and connect
    while (found == true)
    {

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0)
        {
            std::cerr << "WSAStartup failed: " << iResult << std::endl;
            return 1;
        }

        // Create a socket for the client
        ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ConnectSocket == INVALID_SOCKET)
        {
            std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
            WSACleanup();
            return 1;
        }

        // timeout handle
        DWORD timeout = SOCKET_READ_TIMEOUT_SEC * 1000;
        iResult = setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
        if (iResult == SOCKET_ERROR)
        {
            std::cerr << "Error setting receive timeout: " << WSAGetLastError() << std::endl;
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }

        // Set up the server address and port
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(atoi(DEFAULT_PORT));
        serverAddr.sin_addr.s_addr = inet_addr(DEFAULT_SERVER);

        // Connect to server
        SYSTEMTIME currentTime;
        GetLocalTime(&currentTime);

        iResult = connect(ConnectSocket, (sockaddr *)&serverAddr, sizeof(serverAddr));
        // check connection

        if (iResult == SOCKET_ERROR)
        {
            std::cerr << "Error at connect(): " << WSAGetLastError() << std::endl;
            closesocket(ConnectSocket);
            WSACleanup();
            printf("Looking for Server... - %02d:%02d:%02d.%03d\n",
                   currentTime.wHour, currentTime.wMinute, currentTime.wSecond, currentTime.wMilliseconds);
            Sleep(5000); // wait 5 sec then reattempt connection
        }
        else
        {
            printf("Connection Success! - Found at: %02d:%02d:%02d.%03d\n",
                   currentTime.wHour, currentTime.wMinute, currentTime.wSecond, currentTime.wMilliseconds);
            found = false;
        }
    } // while while end

    found = true;

    int choice = 0;
    int t = 0;

    do
    {
        char buffer[DEFAULT_BUFLEN]; // limit
        printf("Select your Data Option: \n");
        printf("Message - 1\n");
        printf("File - 2\n");
        printf("Enter: ");
        std::cin >> choice;
        std::cin.ignore(); // clear

        // send msg
        if (choice == 1)
        {
            // send message
            printf("Enter Message: ");
            while (true)
            {
                std::cin.getline(buffer, DEFAULT_BUFLEN);
                if (strlen(buffer) > 0)
                {
                    break;
                }
                printf("Enter Message: ");
            }

            // Send msg and bytes
            int byteCount = send(ConnectSocket, buffer, strlen(buffer), 0);

            int error_msg = WSAGetLastError();

            if (byteCount == SOCKET_ERROR)
            {
                if (error_msg == WSAETIMEDOUT)
                {
                    std::cerr << "Timeout Error!" << std::endl;
                    break;
                }
                printf("Server send error %ld.\n", error_msg);
                return 1;
            }
            else
            {
                printf("Server: sent %d bytes \n", byteCount);
            }
        }
        else if (choice == 2)
        {
            std::ifstream file("send.txt", std::ios::binary | std::ios::ate);
            if (!file.is_open())
            {
                std::cerr << "Failed to open file!" << std::endl;
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }

            std::streamsize fileSize = file.tellg();
            file.seekg(0, std::ios::beg);

            char *buffer = new char[fileSize];
            file.read(buffer, fileSize);
            file.close();

            iResult = send(ConnectSocket, buffer, fileSize, 0);
            if (iResult == SOCKET_ERROR)
            {
                std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
                delete[] buffer;
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }
            std::cout << "Bytes Sent: " << iResult << std::endl;
        }
        else
        {
            printf("Error! | Invalid Input\n");
        }

        printf("Send another Transmission?: \n");
        printf("Yes --- 1\n");
        printf("No --- 2\n");
        printf("Enter: ");
        std::cin >> t;
        std::cin.ignore();
        if (t == 1)
        {
            printf("---------\n");
        }
        else if (t == 2)
        {
            found = false;
            printf("Closing Socket!");
            closesocket(ConnectSocket);
            WSACleanup();
        }
        else
        {
            // close socket and cleanup.
            found = false;
            printf("Input Invalid!  - Closing Connection.\n");
            closesocket(ConnectSocket);
            WSACleanup();
        }

    } while (found == true);

    return 0;

}
