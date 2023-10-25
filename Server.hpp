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

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include "User.hpp"
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

class User;

class Server {
private:
  Socket _socket;
  std::string _serverName;
  std::string _password;
  std::vector<User> _users;

public:
  // canonical form
  Server(Socket &sock, std::string password);
  ~Server();
  Server(const Server &cpy);
  Server &operator=(const Server &e);

  // methodes
  int initChecker(User &u);
};

#endif
