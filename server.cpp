#include "server.h"

#include <iostream>

Server::Server()
{

}

void Server::start()
{
    server.create_socket("Server", "0.0.0.0", 54000);

    std::cout << "Server started" << std::endl;

    if(server.bind_socket() == -1)
        std::cout << "Failed to bind server socket" << std::endl;

    server.listen_socket();

    client.accept_socket(server.getSocket());

    std::cout << "Client connected. Ready to receive data." << std::endl;


    char buffer[1024];
    int bytesRead;

    while (true)
    {
        while ((bytesRead = recv(client.getSocket(), buffer, sizeof(buffer), 0)) > 0)
        {
            std::cout << "Received data from client: " << buffer << std::endl;
        }
    }

}
