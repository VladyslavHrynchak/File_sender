#ifndef SERVER_H
#define SERVER_H

#include "socket.h"

class Server
{
public:

    Server();

    void start();

private:

    Socket server;

    Socket client;

};

#endif // SERVER_H
