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

#include "Socket.hpp"
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
  std::vector<Channel *> _channelsInvited;
  
public:
  // canonical form
  User(int newSock, char host[NI_MAXHOST], char service[NI_MAXSERV]);
  ~User();
  User(const User &cpy);
  User &operator=(const User &e);

  // setters
  void setNickname(std::string nickname);
  void setUsername(std::string username);
  void setChannelInvited(Channel *channel);

  // getters
  int getPort() { return _port; }
  int getFd() { return _fd; }
  std::string getNickname() { return _nickname; }
  std::string getUsername() { return _username; }
  std::string getHostname() { return _hostname; }
  std::vector<Channel *> &getChannelsInvited() { return _channelsInvited; }
};

#endif
