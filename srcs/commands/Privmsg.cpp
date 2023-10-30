/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 11:02:02 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/27 09:18:09 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Privmsg::Privmsg(Server *srv) : Command(srv) {}

Privmsg::~Privmsg() {}

// syntax: Privmsg <target> [<keys>]

void Privmsg::SendPrivateMessage(User *client, std::vector<std::string> args) {
  std::vector<User *> allUsers = _srv->getUsersOnly();
  for (std::vector<User *>::iterator it = allUsers.begin();
       it != allUsers.end(); it++) {
    if ((*it)->getNickname() == args[1]) {
      send((*it)->getFd(), "[", 1, 0);
      send((*it)->getFd(), client->getNickname().c_str(),
           client->getNickname().length(), 0);
      send((*it)->getFd(), "] ", 2, 0);
      send((*it)->getFd(), args[2].c_str(), args[2].length(), 0);
      send((*it)->getFd(), "\r\n", 2, 0);
    }
    (void)client;
  }
  // gerer cas ou pas trouve username
}

bool Privmsg::execute(User *client, std::vector<std::string> args) {
  if (args.size() < 3) {
    std::cout << "Not enough arguments" << std::endl;
    return false;
  }
  if (args[1][0] == '#') { // send to server
    args[1].erase(0, 1);
    std::cout << "send to channel" << std::endl;
    for (std::map<std::string, Channel *>::iterator it =
             _srv->getChannel().begin();
         it != _srv->getChannel().end(); it++) {
      if (it->first == args[1]) {
        if (!it->second->isInChannel(client)) {
          std::cout << "You are not in this channel" << std::endl;
          return false;
        }
        for (std::vector<User *>::iterator iter =
                 it->second->getUsersOfChannel().begin();
             iter != it->second->getUsersOfChannel().end(); iter++) {
          send((*iter)->getFd(), args[2].c_str(), args[2].length(), 0);
          send((*iter)->getFd(), "\r\n", 2, 0);
        }

      } else {
        std::cout << "This channel doesn't exist" << std::endl;
        return false;
      }
    }

  } else {
    SendPrivateMessage(client, args);
  }
  return true;
}
