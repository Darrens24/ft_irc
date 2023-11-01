/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 12:44:53 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/27 16:08:56 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "colors.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

class Channel;

class User {
private:
  int _fd;
  std::string _hostname;
  int _port;
  std::string _username;
  std::string _nickname;
  std::string _realname;
  std::string _password;
  bool _registered;
  bool _nickRegistered;
  bool _userRegistered;
  std::vector<Channel *> _channelsInvited;
  std::vector<Channel *> _channelsWhereUserIsOperator;

public:
  // constructors
  User(int newSock, char host[NI_MAXHOST], char service[NI_MAXSERV],
       std::string srvpasswd);
  ~User();
  User(const User &cpy);
  User &operator=(const User &e);

  // methods
  void response(const std::string &response);
  bool isInvited(Channel *channel);
  void addChannelWhereUserIsOperator(Channel *channel);
  void removeChannelWhereUserIsOperator(Channel *channel);
  bool isUserOperator(Channel *channel);

  // setters
  void setNickname(std::string nickname) { _nickname = nickname; }
  void setUsername(std::string username) { _username = username; }
  void setRealName(std::string realname) { _realname = realname; }
  void setRegistered() { _registered = true; }
  void setNickRegistered() { _nickRegistered = true; }
  void setUserRegistered() { _userRegistered = true; }
  void setNickUnregistered() { _nickRegistered = false; }
  void setUserUnregistered() { _userRegistered = false; }
  void setChannelInvited(Channel *channel);

  // getters
  std::string getNickname() const { return _nickname; }
  std::string getUsername() const { return _username; }
  bool getRegistered() const { return _registered; }
  bool getNickRegistered() const { return _nickRegistered; }
  bool getUserRegistered() const { return _userRegistered; }
  std::string getPasswd() const { return _password; }
  std::string getHostname() const { return _hostname; }
  int getPort() { return _port; }
  int getFd() { return _fd; }
  std::vector<Channel *> &getChannelsInvited() { return _channelsInvited; }
  std::vector<Channel *> &getChannelsWhereUserIsOperator() {
    return _channelsWhereUserIsOperator;
  }
};
