#ifndef CLIENT_H
#define CLIENT_H

#include "socket.h"

class Client
{
public:

    Client();

    void start();

private:

    Socket client;
};

#endif // CLIENT_H
