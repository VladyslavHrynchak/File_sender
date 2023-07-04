#include "client.h"

#include <iostream>

Client::Client()
{

}

void Client::start()
{
    client.create_socket("Client","127.0.0.1",54000);

    int result = client.connect_socket();

    while(result == -1)
    {
        std::cerr << "Client: failed to connect to server"<< std::endl;
        result = client.connect_socket();
    }

    std::cout << "Connected" << std::endl;
}
