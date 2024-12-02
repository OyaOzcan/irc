#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../Client/Client.hpp"
#include "../Channel/Channel.hpp"
#include "Server.hpp"

class Command {
public:
    Command(Server* server); 

    void PASS(Client &client);
    void NICK(Client &client);
    void USER(Client &client);
    void MODE(Client &client);
    void JOIN(Client &client);
    void INVITE(Client &client);
    void PART(Client &client);
    void TOPIC(Client &client);
    void QUIT(Client &client);
    void WHO(Client &client);
    void PRIVMSG(Client &client);
    void KICK(Client &client);
    void LIST(Client &client);

private:
    Server* server;  

    bool isValidNick(const std::string &nickname);
    bool isValidChannel(const std::string &channel);
};

#endif 
