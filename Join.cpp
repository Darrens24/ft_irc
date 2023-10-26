/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 11:02:02 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/26 16:58:38 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Join::Join(Server *srv) : Command(srv) {}

Join::~Join() {}

// syntax: JOIN <channels> [<keys>]

void Join::execute(User *client, std::vector<std::string> args) {
  std::cout << args[0] << std::endl;
  std::cout << args[1] << std::endl;

  // check if channel exists
  for (std::map<std::string, Channel *>::iterator it =
           _srv->getChannel().begin();
       it != _srv->getChannel().end(); it++) {
    if (it->first == args[1]) {
      if (it->second->isInChannel(client)) {
        std::cout << "You are already in this channel" << std::endl;
        return;
      }
      it->second->addUser(client);
      std::cout << "Join the channel : " << it->first << std::endl;
      std::cout << "owner: " << it->second->getOwner()->getNickname()
                << std::endl;
      return;
    }
  }

  Channel *newChannel = new Channel(args[1]);
  newChannel->addUser(client);
  _srv->getChannel().insert(
      std::pair<std::string, Channel *>(args[1], newChannel));
  newChannel->setOwner(client);
  std::cout << "Channel : '" << newChannel->getChannelName() << "' created by "
            << newChannel->getOwner()->getNickname() << std::endl;
}
