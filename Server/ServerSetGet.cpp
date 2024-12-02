#include "Server.hpp"

void Server :: setUpServerSocket(int server_fd)
{
    this->serverfd = server_fd;

    if (this->serverfd == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
}

int Server::getAccVal()
{
    return this->acc_val;
}