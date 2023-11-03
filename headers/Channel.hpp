/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 12:44:53 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/31 13:54:03 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "User.hpp"
#include "colors.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

class Channel {
private:
  std::string _channelName;
  std::vector<User *> _users;
  User *_owner;
  std::vector<User *> _operators;
  std::string key;
  std::string _topic;
  std::vector<char> _mode;
  unsigned int _limit;

public:
  // constructors
  Channel(std::string channelName);
  ~Channel();
  Channel(const Channel &cpy);
  Channel &operator=(const Channel &e);

  // methods
  int addUser(User *u);
  int isInChannel(User *u);
  void removeUserFromChannel(User *u);
  void addOperator(User *u);
  void removeOperator(User *u);

  void responseALL(std::string response);
  void responseALLnotMe(std::string response, std::string nick);
  void sentMessageToAllMembers(std::string message);

  bool removeMode(char mode);
  bool addMode(char mode);
  bool findMode(char mode);
  std::string getModeString();

  // setters
  void setOwner(User *u);
  void setKey(std::string key);
  void setTopic(std::string topic) { _topic = topic; };
  void setMode(char mode) { _mode.push_back(mode); };
  void setLimit(unsigned int limit) { _limit = limit; };

  // getters
  User *getOwner() { return _owner; }
  std::string getChannelName() { return _channelName; };
  std::vector<User *> &getUsersOfChannel() { return _users; };
  std::string getKey() { return key; };
  std::string getTopic() { return _topic; };
  std::vector<char> getMode() { return _mode; };
  unsigned int getLimit() { return _limit; };
  unsigned int getNumberofUsers() { return _users.size(); };

  // bools
  bool isOperator(User *u);
};
