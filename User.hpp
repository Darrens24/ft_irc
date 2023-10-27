/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 12:44:53 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/26 15:56:20 by feliciencat      ###   ########.fr       */
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

class User {
private:
  int _fd;
  std::string _hostname;
  int _port;
  std::string _username;
  std::string _nickname;
  std::string _password;
  bool _registered;

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
  // int init();
  // int acceptConnection(int socketServer);
  void response(const std::string &response);

  // setters
  void setNickname(std::string nickname);
  void setUsername(std::string username);

  // getters
  std::string getNickname() const { return _nickname; }
  bool getRegistered() const { return _registered; }
  std::string getPasswd() const { return _password; }
  std::string getHostname() const { return _hostname; }
  // int getSocketClient();
  // struct sockaddr_in getAddrClient();
  // socklen_t getAddrClientSize();
  // pollfd *getUserfd();
};

#endif
