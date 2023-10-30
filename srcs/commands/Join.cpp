/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 11:02:02 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/27 16:28:25 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

Join::Join(Server *srv) : Command(srv) {}

Join::~Join() {}

// syntax: JOIN #<channels> [<keys>]

std::vector<std::string> myOwnSplit(std::string str, std::string sep) {
  char *cstr = const_cast<char *>(str.c_str());
  char *current;
  std::vector<std::string> arr;
  current = strtok(cstr, sep.c_str());
  while (current != NULL) {
    arr.push_back(current);
    current = strtok(NULL, sep.c_str());
  }
  return arr;
}

bool Join::execute(User *client, std::vector<std::string> args) {

  if (args.size() < 2 || args.size() > 4) {
    std::cout
        << "construction : 'JOIN #channel1name,#channel2name' 'key1,key2' "
        << std::endl;
    return false;
  }
  if (args[1][0] == '#') {
    args[1].erase(0, 1);
  } else {
    std::cout << "construction : 'JOIN #channelname,#channel2name' ..."
              << std::endl;
    return false;
  }

  bool found_channel = false;
  std::vector<std::string> keys;
  std::vector<std::string> allchannels = myOwnSplit(args[1], ",");
  if (args.size() > 2) {

    args[2] = trim(args[2]);
    keys = myOwnSplit(args[2], ",");
  }

  // map with channel name and key
  std::map<std::string, std::string> channel_key;
  for (std::vector<std::string>::iterator iter = allchannels.begin();
       iter != allchannels.end(); iter++) {
    if (keys.size() > 0) {
      channel_key.insert(std::pair<std::string, std::string>(*iter, keys[0]));
      keys.erase(keys.begin());
    } else {
      channel_key.insert(std::pair<std::string, std::string>(*iter, ""));
    }
  }

  // check if channel exists
  for (std::map<std::string, std::string>::iterator it = channel_key.begin();
       it != channel_key.end(); it++) {
    found_channel = false;
    for (std::map<std::string, Channel *>::iterator iter =
             _srv->getChannel().begin();
         iter != _srv->getChannel().end(); iter++) {
      if (iter->first == it->first) {
        found_channel = true;
        if (iter->second->isInChannel(client)) {
          client->response("You are already in the channel");
          break;
        }
        if (it->second != iter->second->getKey()) {
          client->response(ERR_BADCHANNELKEY(client->getNickname(),
                                             iter->second->getChannelName()));
          break;
        }
        iter->second->addUser(client);
        std::cout << "Join the channel : " << iter->first << std::endl;
        // std::string welcome = "[#" + iter->first + "]" + "[" +
        //                       client->getNickname() +
        //                       "] : Welcome to the channel";
        std::string chan = "#" + iter->first;
        std::string welcome = ":" + client->getNickname() + " JOIN :" + chan;
        client->response(welcome);
        client->response(RPL_TOPIC(client->getNickname(), chan));
        client->response(RPL_NAMREPLY(client->getNickname(), "=", chan));
        client->response(RPL_ENDOFNAMES(client->getNickname(), chan));
        break;
      }
    }
    if (found_channel == false) {
      Channel *newChannel = new Channel(it->first);
      newChannel->addUser(client);
      newChannel->setKey(it->second);
      newChannel->setOwner(client);
      _srv->getChannel().insert(
          std::pair<std::string, Channel *>(it->first, newChannel));
      std::cout << "Channel : '" << newChannel->getChannelName()
                << "' created by " << newChannel->getOwner()->getNickname()
                << std::endl;
      // std::string welcome = "[#" + it->first + "]" + "[" +
      //                       client->getNickname() +
      //                       "] : Welcome to the channel";
      std::string chan = "#" + it->first;
      std::string welcome = ":" + client->getNickname() + " JOIN :" + chan;
      client->response(welcome);
      client->response(RPL_TOPIC(client->getNickname(), chan));
      client->response(RPL_NAMREPLY(client->getNickname(), "=", chan));
      client->response(RPL_ENDOFNAMES(client->getNickname(), chan));
    }
  }
  return true;
}
