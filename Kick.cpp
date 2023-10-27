/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 09:22:04 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/27 10:33:18 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// KICK #channelname username :reason

Kick::Kick(Server *srv) : Command(srv) {}

Kick::~Kick()
{}

void Kick::execute(User *client, std::vector<std::string> args)
{
    if (args.size() < 3) {
        std::cout << "Not enough arguments" << std::endl;
        return;
    }
    
    if (args[1][0] == '#' && args[3][0] == ':') { 
        args[1].erase(0, 1);
        args[3].erase(0, 1);
    }
    else {
        std::cout << "construction : 'KICK #channelname username :reason' " << std::endl;
        return;
    }

    bool found_user = false;
    bool found_channel = false;
    for (std::map<std::string, Channel *>::iterator it =
        _srv->getChannel().begin();
        it != _srv->getChannel().end(); it++)
    {
        if (it->first == args[1])
        {
            found_channel = true;
            if (!it->second->isInChannel(client))
            {
                std::cout << "You are not in this channel" << std::endl;
                return;
            }
            if (it->second->getOwner()->getNickname() != client->getNickname())
            {
                std::cout << "You are not the owner of this channel" << std::endl;
                return;
            }
            for (std::vector<User *>::iterator iter = it->second->getUsersOfChannel().begin() 
                ; iter !=  it->second->getUsersOfChannel().end() ; iter++)
            {
                if ((*iter)->getNickname() == args[2])
                {
                    it->second->getUsersOfChannel().erase(iter);
                    send ((*iter)->getFd(), "you have been kicked by ", 25, 0);
                    send ((*iter)->getFd(), client->getNickname().c_str(), client->getNickname().length(), 0);
                    send ((*iter)->getFd(), " for : ", 8, 0);
                    send ((*iter)->getFd(), args[3].c_str(), args[3].length(), 0);
                    found_user = true;
                    //print user of channel
                    std::cout << "users of channel " << it->first << std::endl;
                    for (std::vector<User *>::iterator iter = it->second->getUsersOfChannel().begin() 
                        ; iter !=  it->second->getUsersOfChannel().end() ; iter++)
                    {
                        std::cout << (*iter)->getNickname() << std::endl;
                    }
                    return;
                }
            }
            if (found_channel == false)
            {
                std::cout << "This channel doesn't exist" << std::endl;
                return;
            }
            if (found_user == false)
            {
                std::cout << "This user is not in this channel" << std::endl;
                return;
            }
            if (it->second->getUsersOfChannel().size() == 0)
            {
                send (client->getFd(), "channel deleted", 15, 0);
                _srv->getChannel().erase(it);
            }
            return;
        }
                
    }
            
}
    