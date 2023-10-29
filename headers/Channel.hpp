/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 12:44:53 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/27 12:25:27 by feliciencat      ###   ########.fr       */
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
  std::string key;

public:
  // canonical form
  Channel(std::string channelName);
  ~Channel();
  Channel(const Channel &cpy);
  Channel &operator=(const Channel &e);

  // methodes
  int addUser(User *u);
  int isInChannel(User *u);
  
  // setters
  void setOwner(User *u);
  void setKey(std::string key);
  
  // getters
  User *getOwner();
  std::string getChannelName();
  std::vector<User *> &getUsersOfChannel();
  std::string getKey();
};
