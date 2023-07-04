#include "socket.h"

#include <iostream>

Socket::Socket()
{

}

Socket::Socket(const std::string &name_, const std::string &ipAddress_, const int port_):name(name_), ipAddress(ipAddress_), port(port_)
{
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);

    if(WSAStartup(DLLVersion, &wsaData) != 0)
    {
        std::cout << "Error" << std::endl;
        exit(1);
    }

    sizeofhint = sizeof(hint);
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    hint.sin_addr.s_addr = inet_addr(ipAddress.c_str());

    sock = socket(AF_INET, SOCK_STREAM, NULL);
}

Socket::~Socket()
{
    closesocket(sock);
}

void Socket::create_socket(const std::string &name_, const std::string &ipAddress_, const int port_)
{
    name = name_;
    ipAddress = ipAddress_;
    port = port_;

    if(sock != INVALID_SOCKET)
    {
        std::cout << name_ << std::endl;
        WSAData wsaData;
        WORD DLLVersion = MAKEWORD(2, 1);
        if(WSAStartup(DLLVersion, &wsaData) != 0)
        {
            std::cout << "Error" << std::endl;
            exit(1);
        }

        sizeofhint = sizeof(hint);
        hint.sin_family = AF_INET;
        hint.sin_port = htons(port);
        hint.sin_addr.s_addr = inet_addr(ipAddress.c_str());

        sock = socket(AF_INET, SOCK_STREAM, NULL);
    }
}

void Socket::close_socket()
{
    closesocket(sock);
}

int Socket::bind_socket()
{
    if(bind(sock, (SOCKADDR*)&hint, sizeof(hint)) == -1)
    {
        std::cerr << name <<": failed to connect to IP/port" << std::endl;
        return -1;
    }

    return 1;
}

void Socket::listen_socket()
{
    if(sock != INVALID_SOCKET)
        listen(sock, SOMAXCONN);
}

int Socket::accept_socket(SOCKET &&accept_socket)
{
    int clientAddressSize = sizeof(hint);
    if ((sock = accept(accept_socket, (struct sockaddr*)&hint, &clientAddressSize)) == INVALID_SOCKET) {
        std::cerr << "Failed to accept incoming connection." << std::endl;
        return -1;
    }

    return 1;
}

int Socket::connect_socket()
{
    int counter_of_connnections = 0;

    int connectRes = connect(sock,(SOCKADDR*)&hint,sizeof(hint));

    while(connectRes == -1 && counter_of_connnections < 10)
    {
        ++counter_of_connnections;


        std::cerr << "Client: failed to connect to server"<< std::endl;
        connectRes = connect(sock,(sockaddr*)&hint,sizeof(hint));
    }
    return connectRes;
}

SOCKET Socket::getSocket()
{
    return sock;
}

void Socket::setSocket(const SOCKET &socket)
{
    sock = std::move(socket);
}

SOCKADDR_IN Socket::getSOCKADDR_IN()
{
    return hint;
}
