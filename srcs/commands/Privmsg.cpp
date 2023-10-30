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

#include "../../headers/Command.hpp"

Privmsg::Privmsg(Server *srv) : Command(srv) {}

Privmsg::~Privmsg() {}

// syntax: Privmsg <target> [<keys>]

void Privmsg::SendPrivateMessage(User *client, std::vector<std::string> args) {
  std::vector<User *> allUsers = _srv->getUsersOnly();
  for (std::vector<User *>::iterator it = allUsers.begin();
       it != allUsers.end(); it++) {
    if ((*it)->getNickname() == args[1]) {
      std::string msg;
      for (long unsigned int i = 2; i < args.size(); i++)
        msg += args[i] + " ";
      std::string message = ":" + client->getNickname() + "!~" +
                            client->getNickname() + "@localhost PRIVMSG " +
                            (*it)->getNickname() + " " + msg;
      (*it)->response(message);
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
        std::string msg;
        std::string channel = "#" + it->second->getChannelName();
        for (long unsigned int i = 2; i < args.size(); i++)
          msg += args[i] + " ";
        std::string message =
            ":" + client->getNickname() + " PRIVMSG " + channel + " " + msg;
        it->second->responseALLnotMe(message, client->getNickname());

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
