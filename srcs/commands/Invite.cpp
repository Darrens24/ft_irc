/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:47:52 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/27 18:50:04 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Invite::Invite(Server *srv) : Command(srv) {}

Invite::~Invite() {}

// INVITE <nickname> <#channel>

bool Invite::execute(User *client, std::vector<std::string> args) {
  if (args.size() < 3) {
    std::cout << "Not enough arguments" << std::endl;
    return false;
  }
  if (args[2][0] == '#') {
    args[2].erase(0, 1);
  } else {
    std::cout << "construction : 'INVITE <nickname> <#channel>' " << std::endl;
    return false;
  }
  Channel *tmpChan = _srv->getChannelByName(args[2]);
  User *tmpUser = _srv->getUserByNickname(args[1]);
  if (tmpChan == NULL) {
    std::cout << "Channel doesn't exist" << std::endl; // ERR_NOSUCHCHANNEL
    return false;
  }
  if (tmpUser == NULL) {
    std::cout << "User doesn't exist" << std::endl; // ??
    return false;
  }
  if (!tmpChan->isInChannel(client)) {
    std::cout << "You are not in this channel" << std::endl; // ERR_NOTONCHANNEL
    return false;
  }
  // if (tmpChan->getOwner()->getNickname() != client->getNickname() &&
  // tmpChan->getMode() != "o")
  // {
  //     std::cout << "You are not the owner of this channel" << std::endl;
  //     return;
  // }
  if (tmpChan->isInChannel(tmpUser)) // ERR_USERONCHANNEL
  {
    std::cout << "User is already in this channel" << std::endl;
    return false;
  }

  tmpUser->setChannelInvited(tmpChan);
  send(tmpUser->getFd(), "You have been invited to join channel : ", 41, 0);
  send(tmpUser->getFd(), tmpChan->getChannelName().c_str(),
       tmpChan->getChannelName().length(), 0);
  send(tmpUser->getFd(), "\n", 2, 0);
  send (tmpUser->getFd(), "If you want copy : JOIN #", 26, 0);
  send (tmpUser->getFd(), tmpChan->getChannelName().c_str(),
         tmpChan->getChannelName().length(), 0);
  send(tmpUser->getFd(), "\n", 2, 0);

  return true;
}
