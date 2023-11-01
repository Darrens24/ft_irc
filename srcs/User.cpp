/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 13:00:52 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/27 16:03:47 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/User.hpp"
#include "../headers/Server.hpp"
#include "../headers/utils.hpp"

/***\ CONSTRUCTORS \***/

User::User(int newSock, char host[NI_MAXHOST], char service[NI_MAXSERV],
           std::string srvpasswd)
    : _fd(newSock), _hostname(host), _port(myAtoi(service)),
      _password(srvpasswd), _registered(false), _userRegistered(false) {
  _username = "";
  _nickname = "";
}

User::~User() {}

User::User(const User &cpy) { *this = cpy; }

User &User::operator=(const User &e) {
  _fd = e._fd;
  _hostname = e._hostname;
  _port = e._port;
  _username = e._username;
  _nickname = e._nickname;
  _realname = e._realname;
  _password = e._password;
  _registered = e._registered;
  _nickRegistered = e._nickRegistered;
  _userRegistered = e._userRegistered;
  _channelsInvited = e._channelsInvited;
  _channelsWhereUserIsOperator = e._channelsWhereUserIsOperator;

  return *this;
}

/***\ METHODS \***/

bool User::isInvited(Channel *channel) {
  for (std::vector<Channel *>::iterator it = _channelsInvited.begin();
       it != _channelsInvited.end(); it++) {
    if (*it == channel)
      return true;
  }
  return false;
}

void User::addChannelWhereUserIsOperator(Channel *channel) {
  _channelsWhereUserIsOperator.push_back(channel);
}

void User::removeChannelWhereUserIsOperator(Channel *channel) {
  for (std::vector<Channel *>::iterator it =
           _channelsWhereUserIsOperator.begin();
       it != _channelsWhereUserIsOperator.end(); it++) {
    if (*it == channel) {
      _channelsWhereUserIsOperator.erase(it);
      return;
    }
  }
}

bool User::isUserOperator(Channel *channel) {
  for (std::vector<Channel *>::iterator it =
           _channelsWhereUserIsOperator.begin();
       it != _channelsWhereUserIsOperator.end(); it++) {
    if (*it == channel)
      return true;
  }
  return false;
}

void User::setChannelInvited(Channel *channel) {
  _channelsInvited.push_back(channel);
}

void User::response(const std::string &response) {
  std::string tmp = response + "\r\n";
  if (!tmp.empty())
    if (send(this->_fd, tmp.c_str(), tmp.length(), 0) < 0)
      (void)tmp; // Une erreur?
}
