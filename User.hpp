/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 12:44:53 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/25 15:10:44 by feliciencat      ###   ########.fr       */
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

public:
  // canonical form
  User(int newSock, char host[NI_MAXHOST], char service[NI_MAXSERV]);
  ~User();
  User(const User &cpy);
  User &operator=(const User &e);

  // methodes
  // int init();
  // int acceptConnection(int socketServer);

  // getters
  // int getSocketClient();
  // struct sockaddr_in getAddrClient();
  // socklen_t getAddrClientSize();
  // pollfd *getUserfd();
};

#endif
