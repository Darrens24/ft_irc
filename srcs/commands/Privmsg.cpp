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

void Privmsg::SendPrivateMessage(User *client, std::vector<std::string> args) {
  std::vector<User *> allUsers = _srv->getUsersOnly();
  for (std::vector<User *>::iterator it = allUsers.begin();
       it != allUsers.end(); it++) {
    if ((*it)->getNickname() == args[1]) {
      std::string msg;
      for (long unsigned int i = 2; i < args.size(); i++)
        msg += args[i] + " ";
      msg = msg.substr(0, msg.size() - 1);
      std::string message = ":" + client->getNickname() + "!" +
                            client->getUsername() + "@:127.0.0.1 PRIVMSG " +
                            (*it)->getNickname() + " " + msg;
      (*it)->response(message);
      return;
    }
  }
  client->response(ERR_NOSUCHNICK(client->getNickname(), args[1]));
  return;
}

bool Privmsg::execute(User *client, std::vector<std::string> args) {
  if (args.size() < 3) {
    client->response(ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG"));
    return false;
  }

  if (args[1][0] == '#') {
    args[1].erase(0, 1);
    for (std::map<std::string, Channel *>::iterator it =
             _srv->getChannel().begin();
         it != _srv->getChannel().end(); it++) {
      if (it->first == args[1]) {
        if (!it->second->isInChannel(client)) {
          client->response("You are not in this channel");
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
        client->response(ERR_NOSUCHCHANNEL(client->getNickname(), args[1]));
        return false;
      }
    }
  } else {
    SendPrivateMessage(client, args);
  }
  return true;
}
