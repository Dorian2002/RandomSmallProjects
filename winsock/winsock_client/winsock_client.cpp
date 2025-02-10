// winsock_server.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#include <iostream>
#include <string>

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

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
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    err = getaddrinfo("127.0.0.1", "13579", &hints, &result);
    if (err) {
        WSACleanup();
        throw std::runtime_error("getaddrinfo failed");
    }

    SOCKET connSocket;
    connSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (connSocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        throw std::runtime_error("socket failed");
    }

    err = connect(connSocket, result->ai_addr, int(result->ai_addrlen));
    if (err == SOCKET_ERROR) {
        closesocket(connSocket);
        WSACleanup();
        throw std::runtime_error("connect failed");
    }

    //Check if still valid
    if (connSocket == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("Unable to connect to server");
    }

    std::string whatImSending = "";

    std::cout << "Type your name : ";
    std::getline(std::cin, whatImSending);
    if (whatImSending.empty()) {
        //Kill client
    }
    send(connSocket, whatImSending.c_str(), strlen(whatImSending.c_str()), 0);

    while (1){
        std::cout << "Type : ";
        std::getline(std::cin,whatImSending);
        if (whatImSending.empty()) {
            send(connSocket, "", 0, 0);
            std::cout << "Closing connection";
            break;
        }
        send(connSocket, whatImSending.c_str(), strlen(whatImSending.c_str()), 0);
    }

    closesocket(connSocket);
    WSACleanup();
    return 0;
}
