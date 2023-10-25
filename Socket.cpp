/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfaria-d <pfaria-d@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 13:24:24 by pfaria-d          #+#    #+#             */
/*   Updated: 2023/10/25 13:24:25 by pfaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "colors.hpp"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

Socket::Socket() : serverSocket(-1), clientSocket(-1), port(-1) {}

Socket::~Socket() { Close(); }

bool Socket::Create(int port) {
  this->port = port;

  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket < 0) {
    cerr << YEL "Socket creation failed" NC << endl;
    return false;
  }

  return true;
}

bool Socket::Bind() {
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(port);

  if (bind(serverSocket, (struct sockaddr *)&serverAddress,
           sizeof(serverAddress)) < 0) {
    cerr << YEL "Bind failed" NC << endl;
    return false;
  }

  return true;
}

bool Socket::Listen(int maxConnections) {
  if (listen(serverSocket, maxConnections) < 0) {
    cerr << YEL "Listen failed" NC << endl;
    return false;
  }

  return true;
}

bool Socket::Accept() {
  sockaddr_in clientAddress;
  socklen_t clientAddressSize = sizeof(clientAddress);

  clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress,
                        &clientAddressSize);
  if (clientSocket < 0) {
    cerr << YEL "Accept failed" NC << endl;
    return false;
  }

  return true;
}

bool Socket::Send(const string &message) {
  if (send(clientSocket, message.c_str(), message.length(), 0) < 0) {
    cerr << YEL "Send failed" NC << endl;
    return false;
  }

  return true;
}

int Socket::Receive(char *buffer, int bufferSize) {
  pollfd fds[1];
  fds[0].fd = clientSocket;
  fds[0].events = POLLIN;

  int ret = poll(fds, 1, 1000);
  if (ret > 0) {
    int receivedBytes = recv(clientSocket, buffer, bufferSize, 0);
    if (receivedBytes < 0) {
      cerr << YEL "Receive failed" NC << endl;
    }
    return receivedBytes;
  }

  return 0;
}

void Socket::Close() {
  if (serverSocket != -1) {
    close(serverSocket);
  }
  if (clientSocket != -1) {
    close(clientSocket);
  }
}
