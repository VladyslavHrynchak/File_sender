#ifndef SOCKET_H
#define SOCKET_H

#include <ws2tcpip.h>
#include <winsock.h>
#include <string>

class Socket
{
public:
    Socket();

    Socket(const std::string &name_,const std::string &ipAddress_,const int port_);

    ~Socket();

    int bind_socket();

    void create_socket(const std::string &name_, const std::string &ipAddress_, const int port_);

    void close_socket();

    void listen_socket();

    int accept_socket(SOCKET &&accept_socket);

    int connect_socket();

    SOCKET getSocket();

    void setSocket(const SOCKET &socket);

    SOCKADDR_IN getSOCKADDR_IN();

protected:
    std::string name;

    std::string ipAddress;

    int port;

    WSAData wsaData;

    SOCKADDR_IN hint;

    SOCKET sock;

    int sizeofhint ;
};

#endif // SOCKET_H
