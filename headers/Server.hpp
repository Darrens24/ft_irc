/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 12:44:53 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/27 16:47:37 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

extern bool server_up;

#include "Channel.hpp"
#include "Command.hpp"
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
#define CLIENTSPEAK(port) "[CLIENT " + port + "]"

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
  std::map<int, std::string> _userport;

public:
  // canonical form
  Server(int port, std::string password);
  ~Server();
  Server(const Server &cpy);
  Server &operator=(const Server &e);

  // getters
  std::vector<User *> getUsersOnly();
  std::map<std::string, Channel *> &getChannels() { return _channels; }
  Channel *getChannelByName(std::string name);
  User *getUserByNickname(std::string nickname);
  std::string getUserPort(int fd);

  // bools
  bool isNicknameAvailable(std::string nickname);

  // init
  void start();
  void acceptNewClient();
  bool getBasicInfo(int fd, std::string str);
  void readFromClient(int fd, int i);
  void launchParser(std::string str, int fd);

  int initChecker(int fd);
  void askUserData(int fd);

  // channels
  int createChannel(std::string name, User *u);
  int joinChannel(std::string name, User *u);
  std::string sendMsgToChannel(std::string target, std::string msg, User *u);
  std::map<std::string, Channel *> &getChannel();
};
