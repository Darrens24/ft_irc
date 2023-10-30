/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 09:22:04 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/30 15:18:13 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// KICK #channelname username :reason

Kick::Kick(Server *srv) : Command(srv) {}

Kick::~Kick() {}

bool Kick::execute(User *client, std::vector<std::string> args) {
  if (args.size() < 3) {
    std::cout << "Not enough arguments" << std::endl;
    return false;
  }

  if (args[1][0] == '#' && args[3][0] == ':') {
    args[1].erase(0, 1);
    args[3].erase(0, 1);
  } else {
    std::cout << "construction : 'KICK #channelname username :reason' "
              << std::endl;
    return false;
  }

  bool found_user = false;

  Channel *tmpChan = _srv->getChannelByName(args[1]);
  if (tmpChan == NULL) {
    std::cout << "Channel doesn't exist" << std::endl;
    return false;
  }
  
  User *tmpUser = _srv->getUserByNickname(args[2]);
  if (tmpUser == NULL) {
    std::cout << "User doesn't exist" << std::endl;
    return false;
  }
  if (!tmpChan->isInChannel(client)) {
    std::cout << "You are not in this channel" << std::endl;
    return false;
  }
  if (tmpChan->getOwner()->getNickname() != client->getNickname()) {
    std::cout << "You are not the owner of this channel" << std::endl;
    return false;
  }
  for (std::vector<User *>::iterator it =
           tmpChan->getUsersOfChannel().begin();
       it != tmpChan->getUsersOfChannel().end(); it++) {
    if ((*it)->getNickname() == args[2]) {
      tmpChan->getUsersOfChannel().erase(it);
      send((*it)->getFd(), "you have been kicked by ", 25, 0);
      send((*it)->getFd(), client->getNickname().c_str(),
           client->getNickname().length(), 0);
      send((*it)->getFd(), " for : ", 8, 0);
      send((*it)->getFd(), args[3].c_str(), args[3].length(), 0);
      found_user = true;
      if (tmpChan->getUsersOfChannel().size() == 0) {
        send(client->getFd(), "channel deleted", 15, 0);
        _srv->getChannel().erase(tmpChan->getChannelName());
      }
      return true;
    }
  }
  if (found_user == false) {
    std::cout << "This user is not in this channel" << std::endl;
    return false;
  }
  return true;
  
}