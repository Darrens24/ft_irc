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
#include "Socket.hpp"
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

class User;

class Server {
private:
  Socket _socket;
  std::string _serverName;
  std::string _password;
  std::vector<User> _users;
  std::map<std::string, Channel> _channels;

public:
  // canonical form
  Server(Socket &sock, std::string password);
  ~Server();
  Server(const Server &cpy);
  Server &operator=(const Server &e);

  // methodes
  int initChecker(User &u);
  int createChannel(std::string name);
  int joinChannel(std::string name, User &u);
};
