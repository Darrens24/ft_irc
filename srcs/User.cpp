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

#include "User.hpp"
#include "Server.hpp"

int myAtoi(char *str) {
  int res = 0;
  int sign = 1;
  int i = 0;

  if (str[0] == '-') {
    sign = -1;
    i++;
  }
  for (; str[i] != '\0'; ++i)
    res = res * 10 + str[i] - '0';
  return sign * res;
}

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
  return *this;
}

void User::setNickname(std::string nickname) { _nickname = nickname; }

void User::setUsername(std::string username) { _username = username; }

void User::setRealName(std::string realname) { _realname = realname; }

void User::setChannelInvited(Channel *channel) {
  _channelsInvited.push_back(channel);
}

void User::response(const std::string &response) {
  std::string tmp = response + "\r\n";
  if (!tmp.empty())
    if (send(this->_fd, tmp.c_str(), tmp.length(), 0) < 0)
      (void)tmp; // Une erreur?
}
