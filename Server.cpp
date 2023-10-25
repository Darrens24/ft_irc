/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 14:57:54 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/25 15:35:26 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(Socket &sock, std::string password)
    : _socket(sock), _password(password) {}

Server::~Server() {
  // Destructeur : libération de ressources si nécessaire
}

Server::Server(const Server &cpy)
    : _serverName(cpy._serverName), _password(cpy._password),
      _users(cpy._users) {
  // Constructeur de copie
}

Server &Server::operator=(const Server &e) {
  if (this == &e) {
    return *this; // retourner *this pour gérer l'affectation à soi-même
  }

  // Copie des membres
  this->_serverName = e._serverName;
  this->_password = e._password;
  this->_users = e._users;

  return *this;
}

int Server::initChecker(User &u) {
  char buffer[1000];
  send(u.getSocketClient(), "Enter password.\n", 17, 0);
  while (1) {
    ssize_t bytes_received = recv(u.getSocketClient(), buffer, 1000, 0);
    if (bytes_received < 0) {
      std::cout << RED "Recv failed" NC << std::endl;
      close(u.getSocketClient());
      return -1;
    }

    for (ssize_t i = 0; i < bytes_received; ++i) {
      if (buffer[i] == '\n' || buffer[i] == '\r') {
        buffer[i] = '\0';
        break;
      }
    }
    std::cout << "Password received: " << buffer << std::endl;
    std::cout << "Password expected: " << _password << std::endl;
    if (!strcmp(buffer, _password.c_str())) {
      send(u.getSocketClient(), "Password OK.\n", 13, 0);
      return 1;
    } else
      send(u.getSocketClient(), "Wrong password.\n", 16, 0);
  }
  return 0;
}
