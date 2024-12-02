#include "Client.hpp"

Client::Client(int fd)
{
    this->userName = "";
    this->nickName= "";
    this->socketFd = fd;
    this->fd = fd;
    this->isAuthenticated = false;
    this->isConnected = false;
}

Client::Client()
{
    this->socketFd = -1;
    this->realIp = -1;

    this->nickName= "";
    this->userName = "";
    this->realName = "";

    this->isAuthenticated = false;
    this->isConnected = false;
}

Client::Client(Client const &copy){*this = copy;}

void    Client::operator=(Client const &copy)
{
    this->socketFd = copy.socketFd;
    this->realIp = copy.realIp;
    this->nickName= copy.nickName;
    this->userName = copy.userName;
    this->realName = copy.realName;
    this->isAuthenticated = copy.isAuthenticated;
    this->isConnected = copy.isConnected;
}

void Client::setSocket(const int socket)
{
    this->socketFd = socket;
}

void Client::setRealName(const std::string &realName)
{
    this->realName = realName;
    std::cout << "user host: " << this->realName << std::endl;
}

void Client::setNickName(const std::string &nickName)
{
    this->nickName= nickName;
    std::cout << "Client " << this->socketFd << " set nickname: " << this->nickName<<std::endl;
    this->print("Welcome, " + this->getNick() + "\n");
}

/* void Client::setUsrName(const std::string &usrName)
{
    this->usrName = usrName;
    std::cout << "Client " << this->socketFd << " set nickname: " << this->usrName <<std::endl;
    this->print("Real name: " + this->usrName + "\n");
} */

void Client::setUserName(const std::string &userName)
{
    this->userName = userName;
    std::cout << "user: " << this->userName << std::endl;
}

void Client::setRealIp(const std::string &realIp)
{
    this->realIp = realIp;
    std::cout << "user ip: " << this->realIp << std::endl;
}

Client::~Client(){}

void Client :: setLoggin(bool status)
{
    std::cout << "Client " << this->socketFd << " logged in the system" << std::endl;
    this->isAuthenticated = status;
}

int Client :: getFd()
{
    return this->fd;
}

void Client :: setFd(int fd)
{
    this->fd = fd;
}

int Client :: getSocket()
{
    return this->socketFd;
}

bool Client :: getLoggedStatus()
{
    return this->isAuthenticated;
}

void Client :: print(std::string str)
{
    send(this->socketFd, str.c_str(), str.length(),  0);
}

void Client :: clientInfo(Client &receive, Client &client)
{
    receive.print(":" + client.getNick() + "!" + client.getUsername() + "@" + client.getRealIp() + " ");
}

std::string Client :: getNick() const
{
    return this->nickName;
}

/* std::string Client :: getName()
{
    return this->usrName;
} */

std::string Client :: getUsername()
{
    return this->userName;
}

std::string Client :: getRealName()
{
    return this->realName;
}

std::string &Client::getRealIp() {return this->realIp;}

bool Client::getisConnected(){return this->isConnected;}

void Client::setisConnected(bool status){this->isConnected = status;}

bool Client::isChannelInvited(std::string  channel)
{
    for (size_t m = 0; m < this->hasInvitationTo.size(); m++)
    {
        if (this->hasInvitationTo[m] ==  channel)
            return true;
    }
    return false;
}
void Client::setInvitationForChannel(std::string channel)
{
    this->hasInvitationTo.push_back(channel);
}

