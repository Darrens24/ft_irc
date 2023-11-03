/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 11:02:02 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/31 14:43:10 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

Join::Join(Server *srv) : Command(srv) {}

Join::~Join() {}

bool Join::addKey(User *client, std::vector<std::string> keys,
                  std::map<std::string, std::string> &channel_key,
                  std::vector<std::string> allchannels) {
  for (std::vector<std::string>::iterator iter = allchannels.begin();
       iter != allchannels.end(); iter++) {
    if ((*iter)[0] == '#') {
      (*iter).erase(0, 1);
    } else {
      client->response(ERR_BADCHANMASK(client->getNickname(), "JOIN"));
      return false;
    }
    if (keys.size() > 0) {
      channel_key.insert(std::pair<std::string, std::string>(*iter, keys[0]));
      keys.erase(keys.begin());
    } else {
      channel_key.insert(std::pair<std::string, std::string>(*iter, ""));
    }
  }
  return true;
}

bool Join::errorJoiningChannel(
    User *client, std::map<std::string, std::string>::iterator it,
    std::map<std::string, Channel *>::iterator iter) {
  if (iter->second->isInChannel(client)) {
    client->response(ERR_USERONCHANNEL(client->getNickname(),
                                       client->getNickname(),
                                       iter->second->getChannelName()));
    return true;
  }
  if (it->second != iter->second->getKey()) {
    client->response(ERR_BADCHANNELKEY(client->getNickname(),
                                       iter->second->getChannelName()));
    return true;
  }
  if (iter->second->findMode('l')) {
    if (iter->second->getNumberofUsers() >= iter->second->getLimit()) {
      client->response(ERR_CHANNELISFULL(client->getNickname(),
                                         iter->second->getChannelName()));
      return true;
    }
  }
  if (iter->second->findMode('i') == true) {
    if (client->isInvited(iter->second) == false) {
      client->response(ERR_INVITEONLYCHAN(client->getNickname(),
                                          iter->second->getChannelName()));
      return true;
    }
  }
  return false;
}

void Join::joinExistingChannel(
    User *client, std::map<std::string, Channel *>::iterator iter) {

  iter->second->addUser(client);

  std::string chan = "#" + iter->first;
  std::string welcome = ":" + client->getNickname() + "!~" +
                        client->getUsername() + "@localhost JOIN " + chan;
  std::string mode = ":" + client->getNickname() + "!~" +
                     client->getNickname() + "@localhost MODE " + chan +
                     iter->second->getModeString();

  iter->second->responseALL(welcome);
  client->response(
      RPL_TOPIC(client->getNickname(), chan, iter->second->getTopic()));

  std::vector<User *> tmpUsers = iter->second->getUsersOfChannel();
  std::string stringUsers = "";
  std::vector<Channel *> tmpChannels = client->getChannelsWhereUserIsOperator();
  Channel *tmpChannel = iter->second;

  for (std::vector<User *>::iterator it = tmpUsers.begin();
       it != tmpUsers.end(); it++) {
    if (tmpChannel->isOperator(*it)) {
      stringUsers += "@" + (*it)->getNickname() + " ";
    } else {
      stringUsers += (*it)->getNickname() + " ";
    }
  }

  iter->second->responseALL(
      RPL_NAMREPLY(client->getUsername(), "=", chan, stringUsers));
  iter->second->responseALL(RPL_ENDOFNAMES(client->getNickname(), chan));
  client->response(mode);
}

void Join::joinNewChannel(User *client,
                          std::map<std::string, std::string>::iterator it) {
  Channel *newChannel = new Channel(it->first);

  newChannel->addUser(client);
  newChannel->addOperator(client);
  client->addChannelWhereUserIsOperator(newChannel);
  newChannel->setKey(it->second);
  if (it->second != "") {
    newChannel->addMode('k');
    std::cout << "mode +k added because key is set" << std::endl;
  }

  newChannel->setOwner(client);
  _srv->getChannel().insert(
      std::pair<std::string, Channel *>(it->first, newChannel));
  std::cout << BLU CLIENTSPEAK(this->_srv->getUserPort(client->getFd()))
            << NC ": Created the channel " << BLU
            << newChannel->getChannelName() << NC << std::endl;

  std::string chan = "#" + it->first;
  std::string welcome = ":" + client->getNickname() + "!~" +
                        client->getUsername() + "@localhost JOIN " + chan;
  std::string mode = ":" + client->getNickname() + "!~" +
                     client->getNickname() + "@localhost MODE " + chan +
                     newChannel->getModeString();

  client->response(welcome);

  std::vector<User *> tmpUsers = newChannel->getUsersOfChannel();
  std::string stringUsers = "";

  for (std::vector<User *>::iterator it = tmpUsers.begin();
       it != tmpUsers.end(); it++) {
    stringUsers += "@" + (*it)->getNickname() + " ";
  }
  client->response(RPL_NAMREPLY(client->getUsername(), "=", chan, stringUsers));
  client->response(RPL_ENDOFNAMES(client->getNickname(), chan));
  client->response(mode);
}

bool Join::execute(User *client, std::vector<std::string> args) {
  if (args.size() < 2 || args.size() > 4) {
    client->response(ERR_NEEDMOREPARAMS(client->getNickname(), "JOIN"));
    return false;
  }

  bool found_channel = false;
  std::vector<std::string> keys;
  std::vector<std::string> allchannels = mySplit(args[1], ",");
  if (args.size() > 2) {

    args[2] = trim(args[2]);
    keys = mySplit(args[2], ",");
  }

  std::map<std::string, std::string> channel_key;
  if (this->addKey(client, keys, channel_key, allchannels) == false) {
    return false;
  }

  for (std::map<std::string, std::string>::iterator it = channel_key.begin();
       it != channel_key.end(); it++) {
    found_channel = false;
    for (std::map<std::string, Channel *>::iterator iter =
             _srv->getChannel().begin();
         iter != _srv->getChannel().end(); iter++) {
      if (iter->first == it->first) {
        found_channel = true;
        if (errorJoiningChannel(client, it, iter) == true) {
          break;
        }
        this->joinExistingChannel(client, iter);
        break;
      }
    }
    if (found_channel == false)
      this->joinNewChannel(client, it);
  }
  return true;
}
