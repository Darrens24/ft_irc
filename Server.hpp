/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 12:44:53 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/25 15:28:23 by feliciencat      ###   ########.fr       */
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

#define SERVERSPEAK "[SERVER]"
#define CLIENTSPEAK "[CLIENT]"

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

  // getters
  std::vector<User *> getUsersOnly();

  // bools
  bool isNicknameAvailable(std::string nickname);

  // methodes
  void start();
  void acceptNewClient();
  bool getBasicInfo(int fd, char buffer[1024]);
  void readFromClient(int fd, int i);
  void launchParser(char buffer[1024], int fd);

  int initChecker(int fd);
  void askUserData(int fd);

  int createChannel(std::string name, User *u);
  int joinChannel(std::string name, User *u);
  std::map<std::string, Channel *> &getChannel();
};
