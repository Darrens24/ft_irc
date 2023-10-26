/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 13:00:52 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/26 15:54:33 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int myAtoi(char *str) {
  int res = 0;
  int sign = 1;
  int i = 0;

  if (str[0] == '-') {
    sign = -1;
    i++;
  }
  for (; str[i] != '\0'; ++i)
    res = res * 10 + str[i] - '0';
  return sign * res;
}

User::User(int newSock, char host[NI_MAXHOST], char service[NI_MAXSERV])
    : _fd(newSock), _hostname(host), _port(myAtoi(service)) {
  _username = "";
  _nickname = "";
}

User::~User() {}

User::User(const User &cpy) { *this = cpy; }

User &User::operator=(const User &e) {
  _fd = e._fd;
  return *this;
}

void User::setNickname(std::string nickname) { _nickname = nickname; }

void User::setUsername(std::string username) { _username = username; }

// int User::init() {
//   _username = "";
//   char buffer[1024] = {0};
//   send(_socketClient, "Enter username.\n", 17, 0);
//   while (_username.empty()) {
//     ssize_t bytes_received = recv(this->_socketClient, buffer, 1024, 0);
//     if (bytes_received < 0) {
//       std::cout << RED "Recv failed" NC << std::endl;
//       close(_socketClient);
//       return -1;
//     }
//     for (ssize_t i = 0; i < bytes_received; ++i) {
//       if (buffer[i] == '\n' || buffer[i] == '\r') {
//         buffer[i] = '\0';
//         break;
//       }
//     }

// _username = std::string(buffer);
// if (_username.empty()) {
//   std::cout << "Username cannot be empty. Please enter again." << std::endl;
//   send(_socketClient, "Username cannot be empty. Please enter again.\n", 44,
//   0);
// } else {
//   std::cout << "username is : " << _username << std::endl;
//   send(_socketClient, "username is : ", 15, 0);
//   send(_socketClient, _username.c_str(), _username.length(), 0);
//   send(_socketClient, "\n", 1, 0);
//   break;
// }
// }
//
// return 0;
// }
//
// int User::acceptConnection(int socketServer) {
//   if ((_socketClient = accept(socketServer, (struct sockaddr *)&_addrClient,
//                               &_addrClientSize)) < 0) {
//     std::cout << RED "Accept failed" NC << std::endl;
//     return 1;
//   }
//   return (0);
// }

// int User::getSocketClient() { return _socketClient; }
//
// struct sockaddr_in User::getAddrClient() { return _addrClient; }
//
// socklen_t User::getAddrClientSize() { return _addrClientSize; }
//
// pollfd *User::getUserfd() { return _userfd; }
