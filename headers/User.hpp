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

#ifndef USER_HPP
#define USER_HPP

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
<<<<<<< HEAD
=======
  bool _nickRegistered;
>>>>>>> edouard
  bool _userRegistered;
  std::vector<Channel *> _channelsInvited;

public:
  // canonical form
  User(int newSock, char host[NI_MAXHOST], char service[NI_MAXSERV]);

public:
  // canonical form
  User(int newSock, char host[NI_MAXHOST], char service[NI_MAXSERV],
       std::string srvpasswd);
  ~User();
  User(const User &cpy);
  User &operator=(const User &e);

  // methodes
  void response(const std::string &response);

  // setters
  void setNickname(std::string nickname);
  void setUsername(std::string username);
  void setRealName(std::string realname);
  void setRegistered() { _registered = true; }
  void setNickRegistered() { _nickRegistered = true; }
  void setUserRegistered() { _userRegistered = true; }
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
};

#endif