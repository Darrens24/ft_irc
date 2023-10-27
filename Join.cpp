/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 11:02:02 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/27 12:53:59 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

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

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

void Join::execute(User *client, std::vector<std::string> args) {

  if (args.size() < 2 || args.size() > 4) {
    std::cout << "construction : 'JOIN #channel1name,#channel2name' 'key1,key2' " << std::endl;
    return;
  }
  if (args[1][0] == '#') {
    args[1].erase(0, 1);
  } else {
    std::cout << "construction : 'JOIN #channelname,#channel2name' ..." << std::endl;
    return;
  }

  bool found_channel = false;
  std::vector<std::string> allchannels = myOwnSplit(args[1], ","); 
  //trim args[2] spaces
  args[2] = trim(args[2]);
  
  std::vector<std::string> keys = myOwnSplit(args[2], ",");
  
  //map with channel name and key
  std::map<std::string, std::string> channel_key;
  for (std::vector<std::string>::iterator iter = allchannels.begin();
       iter != allchannels.end(); iter++)
  {
    
    if (keys.size() > 0)
    {
      channel_key.insert(std::pair<std::string, std::string>(*iter, keys[0]));
      keys.erase(keys.begin());
    }
    else
    {
      channel_key.insert(std::pair<std::string, std::string>(*iter, ""));
    }
  }
  // check if channel exists
  for(std::map<std::string, std::string>::iterator it = channel_key.begin(); it != channel_key.end(); it++)
  {
    std::cout << "channel name: " << it->first << std::endl;
    std::cout << "channel key: " << it->second << std::endl;
  }
  for (std::map<std::string, std::string>::iterator it = channel_key.begin();
       it != channel_key.end(); it++)
  {
    found_channel = false;
    for (std::map<std::string, Channel *>::iterator iter =
             _srv->getChannel().begin();
         iter != _srv->getChannel().end(); iter++)
    {
      if (iter->first == it->first)
      {
        found_channel = true;
        if (iter->second->isInChannel(client))
        {
          std::cout << "You are already in channel : " << iter->second->getChannelName() << std::endl;
          break;
        }
        std::cout << "-------------------------" << std::endl;
        std::cout << "key: " << it->second << std::endl;
        std::cout << "key of channel: " << iter->second->getKey() << std::endl;
        std::cout << "-------------------------" << std::endl;

        if (it->second != iter->second->getKey())
        {
          std::cout << "Wrong key for channel : " << iter->second->getChannelName() << std::endl;
          break;
        }
        iter->second->addUser(client);
        std::cout << "Join the channel : " << iter->first << std::endl;
        std::cout << "owner: " << iter->second->getOwner()->getNickname()
                  << std::endl;
        break;
      }
    }
    if (found_channel == false)
    {
      Channel *newChannel = new Channel(it->first);
      newChannel->addUser(client);
      newChannel->setKey(it->second);
      newChannel->setOwner(client);
      _srv->getChannel().insert(
          std::pair<std::string, Channel *>(it->first, newChannel));
      std::cout << "Channel : '" << newChannel->getChannelName() << "' created by "
                << newChannel->getOwner()->getNickname() << std::endl;
    }
  }
    //print all server channels
  std::cout << "-----------------------" << std::endl;
  std::cout << "all channels of server " << std::endl;
  for (std::map<std::string, Channel *>::iterator it =
          _srv->getChannel().begin();
      it != _srv->getChannel().end(); it++)
  {
    std::cout << it->first << std::endl;
  }
  std::cout << "-----------------------" << std::endl;

}
