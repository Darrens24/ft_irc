/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 11:02:02 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/26 20:21:34 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Privmsg::Privmsg(Server *srv) : Command(srv) {}

Privmsg::~Privmsg() {}

// syntax: Privmsg <target> [<keys>]

void Privmsg::execute(User *client, std::vector<std::string> args) {
    if (args.size() < 3) {
        std::cout << "Not enough arguments" << std::endl;
        return;
    }
    if (args[1][0] == '#') {
        args[1].erase(0, 1);
        // check if channel exists
        for (std::map<std::string, Channel *>::iterator it =
                     _srv->getChannel().begin();
             it != _srv->getChannel().end(); it++) {
            if (it->first == args[1]) {
                if (!it->second->isInChannel(client)) {
                    std::cout << "You are not in this channel" << std::endl;
                    return;
                }
                for (std::vector<User *>::iterator iter = it->second->getUsersOfChannel().begin() 
                    ; iter !=  it->second->getUsersOfChannel().end() ; iter++) {
                        send((*iter)->getFd(), args[2].c_str(), args[2].length(), 0);
                    }
                    
            }
            else {
                std::cout << "This channel doesn't exist" << std::endl;
                return;
            }
            
        }

        return;
    }
}