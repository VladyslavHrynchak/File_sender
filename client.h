#ifndef CLIENT_H
#define CLIENT_H

#include "socket.h"
#include <vector>

class Client
{
public:

    Client();

    void start();

private:

    void sendFileData(const std::vector<uint8_t>& fileData);

    std::string openFileExplorer();

    std::vector<uint8_t> readFileBytes(const std::string& filename);

    Socket client;
};

#endif // CLIENT_H
