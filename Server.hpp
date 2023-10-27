/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 12:44:53 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/27 14:42:19 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Channel.hpp"
#include "Command.hpp"
#include "Socket.hpp"
#include "User.hpp"
#include "colors.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

class User;

class Server {
private:
  std::string _serverName;
  std::string _password;
  int _port;
  int _socket;
  int _opt;
  int _maxClients;
  struct sockaddr_in _address;
  socklen_t _addrLen;
  std::map<int, User *> _users;
  std::vector<pollfd> _polls;
  std::map<std::string, Channel *> _channels;

public:
  // canonical form
  Server(int port, std::string password);
  ~Server();
  Server(const Server &cpy);
  Server &operator=(const Server &e);

  // methodes
  void start();
  void acceptNewClient();
  void readFromClient(int fd, int i);
  void launchParser(char buffer[1024], int fd);

  int initChecker(int fd);
  void askUserData(int fd);
  Channel *getChannelByName(std::string name);

  int createChannel(std::string name, User *u);
  int joinChannel(std::string name, User *u);
  std::string sendMsgToChannel(std::string target ,std::string msg, User *u);
  std::map<std::string, Channel *> &getChannel();
  std::vector<User *> getUsersOnly();
};
