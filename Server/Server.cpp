#include "Server.hpp"

Server::Server()
{
    this->password = "";
    this->serverIP = "";
    this->optv = 1;
}

Server::Server(Server const &server){*this = server;}
void    Server::operator=(Server const &server)
{
    this->acc_val = server.acc_val;
    this->acceptfd = server.acceptfd;
    this->adr_len = server.adr_len;
    this->channels = server.channels;
    this->clients = server.clients;
    this->commands = server.commands;
    this->fds = server.fds;
    this->optv = server.optv;
    this->password = server.password;
    this->port = server.port;
    this->server_address = server.server_address;
    this->serverfd = server.serverfd;
    this->serverIP = server.serverIP;
    this->sockfd = server.sockfd;
}

Server::Server(int port, std::string password)
{
    this->port = port;
    this->password = password;
    this->optv = 1;

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    this->server_address.sin_addr.s_addr = INADDR_ANY;
    this->serverIP = inet_ntoa(this->server_address.sin_addr);
    this->adr_len = sizeof(this->server_address);
}

void Server :: acceptClient()
{
    struct pollfd connect;

    if (this->fds[0].revents & POLLIN)
    {
        checkAcceptStatus(accept(this->serverfd, (sockaddr *)&this->server_address, &this->adr_len));
        
        Client client(this->acc_val);
        this->clients.push_back(client);
        this->clients[this->acc_val - 4].setFd(this->acc_val);

        this->clients[this->acc_val - 4].setSocket(this->acc_val);
        unsigned long clientAddr = ntohl(this->server_address.sin_addr.s_addr);
        std::string clientIP = std::to_string((clientAddr >> 24) & 0xFF) + "." + std::to_string((clientAddr >> 16) & 0xFF) + "." + std::to_string((clientAddr >> 8) & 0xFF) + "." + std::to_string(clientAddr & 0xFF);
        this->clients[this->acc_val - 4].setRealIp(clientIP);

        std::cout << "Client " << this->acc_val << " connected successfully" << std::endl;
        this->fds.push_back(connect);
        this->fds[this->acc_val - 3].fd = this->acc_val;
        this->fds[this->acc_val - 3].events = POLLIN;
    }
}

void Server :: start()
{
    this->setUpServerSocket(socket(AF_INET, SOCK_STREAM, 0));
    setsockopt(this->serverfd, SOL_SOCKET, SO_REUSEADDR, &this->optv, sizeof(this->optv));
    
    checkBindStatus(bind(this->serverfd, (const sockaddr *)&this->server_address, this->adr_len));
    checkListenStatus(listen(this->serverfd, 5));
    
    struct pollfd server;
	this->fds.push_back(server);
    this->fds[0].fd = this->serverfd;
    this->fds[0].events = POLLIN;


    std::cout << "Server is listening..." << std::endl;
}

void Server :: runServer()
{
    std::vector<std::string> bufferRaw;

    this->start();

    while(1)
    {
        checkPollStatus(poll(&(this->fds[0]), this->fds.size(), -1));
        acceptClient();
        for (size_t i = 1; i < this->fds.size(); i++)
        {
            if (this->fds[i].fd != -1 && this->fds[i].revents & POLLIN) {
                char buff[1024] = {0}; 
                if (checkRecvStatus(recv(this->fds[i].fd, buff, sizeof(buff), 0), i) == 1)
                {
                    std::cout << "client message: " << buff << std::endl;
                    parseMessage(buff);
                    executeCommands(i);
                }
            }
        }

    }
    close(this->acc_val);
    close(this->serverfd);
}

bool Server :: isChannelExist(std::string channel)
{
    for (size_t i = 0; i < this->channels.size(); i++)
    {
        if (this->channels[i].getName() == channel)
            return true;
    }

    return false;
}

size_t Server::getClientIndex(std::string clientNick, Channel &channel)
{
    size_t i = 0;
    for(; i < channel.getMembers().size(); i++)
    {
        if(channel.getMembers()[i].getNick() == clientNick)
            return i;
    }
    return i;
}

size_t Server :: getChannelIndex(std::string channel)
{
    size_t i = 0;

    for (; i < this->channels.size(); i++)
    {
        if (this->channels[i].getName() == channel)
            return i;
    }

    return i;
}

void Server :: addToChannel(Channel &channel, Client &client)
{
    std::string symbol;
    std::string prefix;

    if (channel.isMember(client) == false)
    {
        channel.addMembers(client);
        for (size_t m = 0; m < channel.getMembers().size(); m++)
        {
            client.clientInfo(channel.getMembers()[m], client);
            channel.getMembers()[m].print("JOIN " + channel.getName() + "\r\n");
        }
        if (channel.getTopic() != "")
        {
            client.clientInfo(client, client);
            client.print(client.getNick() + " " + channel.getName() + " :" + channel.getTopic());
        }
        client.clientInfo(client, client);
        client.print(client.getNick() + " = " + channel.getName()  + " :");
        channel.isOperator(client) ? prefix = "@" : prefix = "v";
        client.print(prefix + client.getNick());
        for (size_t m = 0; m < channel.getMembers().size() - 1; m++)
        {
            client.print(" ");
            channel.isOperator(channel.getMembers()[m]) ? prefix = "@" : prefix = "v";
            client.print(prefix + channel.getMembers()[m].getNick());
        }
        client.print("\r\n");
        client.clientInfo(client, client);
        client.print(client.getNick() + " " + channel.getName() + " :End of /NAMES list\r\n");
        for (size_t m = 0; m < channel.getMembers().size() - 1; m++)
        {
            client.clientInfo(client, channel.getMembers()[m]);
            client.print("JOIN " + channel.getName() + "\r\n");
        }
    }
    else
        client.print("You are already in the channel " + channel.getName() + "\r\n");
}

Server::~Server(){}

std::string Server :: getServerIP(void)
{
    return this->serverIP;
}

