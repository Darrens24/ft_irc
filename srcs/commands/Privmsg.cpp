/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 11:02:02 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/31 12:38:40 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"
//#include "../../headers/Channel.hpp"

Privmsg::Privmsg(Server *srv) : Command(srv) {}

Privmsg::~Privmsg() {}

///PRIVMSG Bob,Charlie,#example_channel :Hello!

std::vector<std::string> myOwnSplit_msg(std::string str, std::string sep) {
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

void Privmsg::SendPrivateMessage(User *client, std::string target, std::vector<std::string> args) {
  std::vector<User *> allUsers = _srv->getUsersOnly();
  for (std::vector<User *>::iterator it = allUsers.begin();
       it != allUsers.end(); it++) {
    if ((*it)->getNickname() == target) {
      std::string msg;
      for (long unsigned int i = 2; i < args.size(); i++)
        msg += args[i] + " ";
      std::string message = ":" + client->getNickname() + "!~" +
                            client->getNickname() + "@localhost PRIVMSG " +
                            (*it)->getNickname() + " " + msg;
      (*it)->response(message);
      return;
    }
  }
  client->response(ERR_NOSUCHNICK(client->getNickname(), target));
  return;
}

bool Privmsg::execute(User *client, std::vector<std::string> args) {
  if (args.size() < 3) {
    client->response(ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG"));
    return false;
  }
  std::vector<std::string> all_targets = myOwnSplit_msg(args[1], ",");
  if (all_targets.size() == 0)
  {
    client->response(ERR_NORECIPIENT(client->getNickname(), "PRIVMSG"));
    return false;
  }
  for (std::vector<std::string> ::iterator it = all_targets.begin(); it != all_targets.end(); it++)
  {
    if (it->length() == 0)
    {
      client->response(ERR_NORECIPIENT(client->getNickname(), "PRIVMSG"));
      return false;
    }
    else
    {
      if (execute_msg(client, *it, args) == false)
        return false;
    }
  }
  return true;
}

bool Privmsg::execute_msg(User *client, std::string target, std::vector<std::string> args) {
  if (target[0] == '#')
  {
    target.erase(0, 1);
     
    Channel *tmpChan = _srv->getChannelByName(target);
    if(tmpChan == NULL)
    {
      client->response(ERR_NOSUCHCHANNEL(client->getNickname(), target));
      return false;
    }
    if (!tmpChan->isInChannel(client)) {
      client->response(ERR_NOTONCHANNEL(client->getNickname(), target));
      return false;
    }
    std::string msg;
    std::string channel = "#" + tmpChan->getChannelName();

    for (long unsigned int i = 2; i < args.size(); i++)
    {
      msg += args[i] + " ";
    }
    std::string message =
        ":" + client->getNickname() + " PRIVMSG " + channel + " " + msg;
    std::cout << message << std::endl;
    tmpChan->responseALLnotMe(message, client->getNickname());

  } else {
    SendPrivateMessage(client, target, args);
  }
  return true;
}
