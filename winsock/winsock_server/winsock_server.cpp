// winsock_server.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <vector>
#include <map>

#pragma comment(lib, "Ws2_32.lib")

std::map<std::string, SOCKET> gClients;

void listenForConnection(SOCKET* listenSocket) {
    int err = 0;
    std::cout << "Listening..." << std::endl;
    while (1) {
        listen(*listenSocket, SOMAXCONN); // Listen to the socket with no max connection
        if (err == SOCKET_ERROR) {
            closesocket(*listenSocket);
            WSACleanup();
            throw std::runtime_error("listen failed");
        }
        //if exit from listen() (execution coutinues), there is a connection

        SOCKET clientSocket = INVALID_SOCKET;
        clientSocket = accept(*listenSocket, nullptr, nullptr); //Accept the connection and redirect it to a new socket dedicated to this client (random port)

        if (clientSocket == INVALID_SOCKET) {
            WSACleanup();
            std::runtime_error("accept failed");
        }

        char temp[512];
        ZeroMemory(temp, 512);
        int bytesReceived = recv(clientSocket, temp, 512, 0);
        std::string name = static_cast<std::string>(temp);
        if (bytesReceived > 0) {
            gClients.emplace(name,clientSocket);
        }
        else if (bytesReceived == 0) {
            std::cout << "Closing" << std::endl;
            break;
        }
        else {
            std::cout << "Recv failed" << std::endl;
            break;
        }

        std::cout << "New client connected : " << name << ", total clients : " << gClients.size() << std::endl;
    }
    closesocket(*listenSocket); //Close listen socket anyway
}

int main(int argc, char** argv)
{
    WSADATA wsaData;
    int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err)
        throw std::runtime_error("WSAStartup failed");

    struct addrinfo* result = nullptr,
                   * ptdr = nullptr,
                     hints;

    ZeroMemory(&hints, sizeof hints);// Fill hints with 0 in memory
    hints.ai_family = AF_INET;// UDP/TCP
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    err = getaddrinfo(
        nullptr, // We don't want to connect to anyone, we're the server
        "13579", // Port
        &hints,  // What we want
        &result  // What we get
    );

    if (err) {
        WSACleanup();
        throw std::runtime_error("getaddrinfo failed");
    }

    SOCKET listenSocket = INVALID_SOCKET;
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol); //Configure the socket with the addrinfo result
    if (listenSocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        throw std::runtime_error("socket failed");
    }

    err = bind(listenSocket, result->ai_addr, int(result->ai_addrlen)); // Bind the socket to the addrinfo result
    freeaddrinfo(result); // Socket is binded, we don't need the addrinfo result anymore
    if (err) {
        closesocket(listenSocket); //Close the socket to free it as it's not gonna be used
        WSACleanup();
        throw std::runtime_error("bind failed");
    }

    std::jthread thread = std::jthread(listenForConnection, &listenSocket);



    while (1) {
        std::map<std::string, SOCKET>::iterator it;
        for (it = gClients.begin(); it != gClients.end(); ++it) {
            char temp[512];
            ZeroMemory(temp, 512);
            int bytesReceived = recv(it->second, temp, 512, 0);
            if (bytesReceived > 0) {
                std::cout << temp << std::endl;
            }
            else if (bytesReceived == 0) {
                std::cout << "Closing" << std::endl;
                break;
            }
            else {
                std::cout << "Recv failed" << std::endl;
                break;
            }
        }
    }
    std::map<std::string, SOCKET>::iterator it;
    for (it = gClients.begin(); it != gClients.end(); ++it) {
        closesocket(it->second);
    }
    closesocket(listenSocket);
    WSACleanup();
    return 0;
}

