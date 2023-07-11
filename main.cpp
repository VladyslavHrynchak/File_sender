#include <iostream>
#include <thread>

#include "server.h"
#include "client.h"

using namespace std;

int main()
{
    thread th1([]()
               {
                   Server server;
                   server.start();
               });

    thread th2([]()
               {
                   Client client;
                   client.start();
               });

    th1.join();
    th2.join();

    system("pause");

    return 0;
}
