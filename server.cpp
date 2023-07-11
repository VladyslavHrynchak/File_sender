#include "server.h"

#include <iostream>
#include <fstream>

#define MAX_BUFFER_SIZE 1024

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

    if(client.accept_socket(server.getSocket()) == -1)
        std::cout << "Failed to accept incoming connection." << std::endl;

    else
        std::cout << "Client connected. Ready to receive data." << std::endl;


    char buffer_path[MAX_BUFFER_SIZE];

    char buffer_file[8192];

    int size_of_file = 0 , size_of_file_ntohl = 0;

    int bytesRead_size, bytesRead_write = 0, bytesRead_file, bytesRead_path;

    std::ofstream file;

    bool stop = false;

    while(!stop)
    {
        if((bytesRead_path = recv(client.getSocket(), buffer_path, sizeof(buffer_path) - 1, 0)) > 0)
        {
            buffer_path[bytesRead_path] = '\0';

            file.open(buffer_path, std::ios::binary);

            if (!file)
            {
                std::cout << buffer_path << std::endl;
                std::cerr << "Failed to create file." << std::endl;
                client.close_socket();
                return ;
            }

       }

        if((bytesRead_size = recv(client.getSocket(), reinterpret_cast<char*>(&size_of_file),  sizeof(size_of_file), 0)) > 0)
        {
            size_of_file_ntohl = ntohl(size_of_file);
        }

        while ((bytesRead_file = recv(client.getSocket(), buffer_file, sizeof(buffer_file), 0)) > 0 && (bytesRead_write != size_of_file_ntohl))
        {
            bytesRead_write += bytesRead_file;

            file.write(buffer_file, bytesRead_file);

            stop = true;
        }
    }

    std::cout << "File was closed." << std::endl;

    file.close();
    client.close_socket();

}
