/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 13:00:52 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/25 15:10:54 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(Socket &sock) {
  _socketClient = sock.getClientSocket();
  _addrClientSize = sizeof(_addrClient);
  _userfd[0].fd = _socketClient;
  _userfd[0].events = POLLIN;
  _ret = poll(_userfd, 1, 1000);
}

User::~User() {}

User::User(const User &cpy) { *this = cpy; }

User &User::operator=(const User &e) {
  if (this == &e)
    return (*this);
  this->_addrClient = e._addrClient;
  this->_addrClientSize = e._addrClientSize;
  this->_socketClient = e._socketClient;
  this->_userfd[0].fd = e._userfd[0].fd;
  this->_userfd[0].events = e._userfd[0].events;
  this->_username = e._username;
  return (*this);
}

int User::init() {
  _username = "";
  send(_socketClient, "Enter username.\n", 17, 0);
  while (_username.empty()) {
    char buffer[1024] = {0};

    if (_ret > 0) {
      ssize_t bytes_received = recv(_socketClient, buffer, 1024, 0);
      if (bytes_received < 0) {
        std::cout << RED "Recv failed" NC << std::endl;
        close(_socketClient);
        return 1;
      }

      for (ssize_t i = 0; i < bytes_received; ++i) {
        if (buffer[i] == '\n' || buffer[i] == '\r') {
          buffer[i] = '\0';
          break;
        }
      }

      _username = std::string(buffer);
      if (_username.empty()) {
        std::cout << "Username cannot be empty. Please enter again."
                  << std::endl;
        send(_socketClient, "Username cannot be empty. Please enter again.\n",
             44, 0);
      } else {
        send(_socketClient, "username is : ", 15, 0);
        send(_socketClient, _username.c_str(), _username.length(), 0);
        break;
      }
    }
  }
  close(_socketClient);
  return 0;
}

int User::acceptConnection(int socketServer) {
  if ((_socketClient = accept(socketServer, (struct sockaddr *)&_addrClient,
                              &_addrClientSize)) < 0) {
    std::cout << RED "Accept failed" NC << std::endl;
    return 1;
  }
  return (0);
}

int User::getSocketClient() { return _socketClient; }

struct sockaddr_in User::getAddrClient() { return _addrClient; }

socklen_t User::getAddrClientSize() { return _addrClientSize; }

pollfd *User::getUserfd() { return _userfd; }
