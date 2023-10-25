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

Socket::Socket(const std::string &pass)
    : serverSocket(-1), clientSocket(-1), port(-1), password(pass) {}

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

void Socket::CloseClient() {
  if (clientSocket != -1) {
    close(clientSocket);
    clientSocket = -1;
  }
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

  int failedAttempts = 0;

  while (failedAttempts < 3) {
    string passwordRequest = "Password: ";
    if (send(clientSocket, passwordRequest.c_str(), passwordRequest.length(),
             0) < 0) {
      cerr << YEL "Send failed" NC << endl;
      Close();
      return false;
    }

    char password[1024];
    int receivedBytes = recv(clientSocket, password, 1024, 0);
    if (receivedBytes < 0) {
      cerr << YEL "Receive failed" NC << endl;
      Close();
      return false;
    }

    password[receivedBytes - 1] = '\0';
    if (string(password) == this->password) {
      string authenticationSuccess =
          "Authentication successful. You are now logged in.\n";
      if (send(clientSocket, authenticationSuccess.c_str(),
               authenticationSuccess.length(), 0) < 0) {
        cerr << YEL "Send failed" NC << endl;
        Close();
        return false;
      }
      return true;
    } else {
      failedAttempts++;
      string incorrectPassword = "Incorrect Password. Please try again.\n";
      if (send(clientSocket, incorrectPassword.c_str(),
               incorrectPassword.length(), 0) < 0) {
        cerr << YEL "Send failed" NC << endl;
        Close();
        return false;
      }
    }
  }
  string incorrectPassword = "Three failed attempts. Disconnecting.";
  if (send(clientSocket, incorrectPassword.c_str(), incorrectPassword.length(),
           0) < 0) {
    cerr << YEL "Send failed" NC << endl;
    Close();
    return false;
  }
  CloseClient();
  return false;
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
  if (clientSocket != -1) {
    close(clientSocket);
  }
  if (serverSocket != -1) {
    close(serverSocket);
  }
}
