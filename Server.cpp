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
#include <sys/types.h>

std::vector<std::string> mySplit(std::string str, std::string sep) {
  char *cstr = const_cast<char *>(str.c_str());
  char *current;
  std::vector<std::string> arr;
  current = strtok(cstr, sep.c_str());
  while (current != NULL) {
    arr.push_back(current);
    current = strtok(NULL, sep.c_str());
  }
  return arr;
}

Server::Server(int port, std::string password)
    : _serverName("our-IRC"), _password(password), _port(port), _opt(1),
      _maxClients(30) {

  this->_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->_socket == -1) {
    std::cout << RED "Socket creation failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << GRN "Socket created" NC << std::endl;

  if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &this->_opt,
                 sizeof(int))) {
    std::cout << RED "Setsockopt failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << GRN "Socket reusable" NC << std::endl;

  if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0) {
    std::cout << RED "Fcntl failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << GRN "Socket non-blocking" NC << std::endl;

  this->_address.sin_family = AF_INET;
  this->_address.sin_addr.s_addr = INADDR_ANY;
  this->_address.sin_port = htons(port);
  this->_addrLen = sizeof(this->_address);

  if (bind(this->_socket, (struct sockaddr *)&this->_address,
           sizeof(this->_address)) < 0) {
    std::cout << RED "Bind failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << GRN "Socket binded" NC << std::endl;

  if (listen(this->_socket, this->_maxClients) < 0) {
    std::cout << RED "Listen failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << GRN "Server listening on port " << port << NC << std::endl;
}

Server::~Server() {}

Server::Server(const Server &cpy)
    : _serverName(cpy._serverName), _password(cpy._password),
      _users(cpy._users) {}

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

void Server::start() {
  pollfd serverPoll;
  serverPoll.fd = this->_socket;
  serverPoll.events = POLLIN | POLLHUP | POLLRDHUP;
  serverPoll.revents = 0;

  this->_polls.push_back(serverPoll);
  std::cout << GRN "Poll server created" NC << std::endl;

  while (1) {
    int pollCount = poll(&this->_polls[0], this->_polls.size(), -1);
    if (pollCount < 0) {
      std::cout << RED "Poll failed" NC << std::endl;
      exit(EXIT_FAILURE);
    }

    for (long unsigned int i = 0; i < this->_polls.size(); i++) {
      if (this->_polls[i].revents & POLLRDHUP) {
        std::cout << RED "Client disconnected" NC << std::endl;
        close(this->_polls[i].fd);
        this->_polls.erase(this->_polls.begin() + i);
        break;
      }
      if (this->_polls[i].revents & POLLIN) {
        if (this->_polls[i].fd == this->_socket) {
          this->acceptNewClient();
          break;
        } else if (this->_polls[i].fd) {
          this->readFromClient(this->_polls[i].fd, i);
          break;
        }
      }
    }
  }
}

void Server::readFromClient(int fd, int i) {
  char buffer[1024];
  // memset(buffer, 0, 1024);

  ssize_t read = recv(fd, buffer, 1024, 0);
  if (read < 0) {
    std::cout << RED "Recv failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  if (read == 0) {
    std::cout << RED "Client disconnected (read)" NC << std::endl;
    close(this->_polls[i].fd);
    this->_polls.erase(this->_polls.begin() + i);
    return;
  }

  launchParser(buffer, fd);
}

void Server::launchParser(char buffer[1024], int fd) {
  std::string str(buffer);
  (void)fd;
  std::vector<std::string> array = mySplit(str, "\r\n\t\v ");

  // std::cout << "Message is : " << buffer << std::endl;
  if (array.size() == 0) {
    return;
  }
  if (this->_users.find(fd) != this->_users.end())
    std::cout << "User found" << std::endl;

  if (array[0] == "JOIN") {
    Join join(this);
    join.execute(this->_users[fd], array);
  }
}

void Server::acceptNewClient() {
  int fd = accept(this->_socket, (struct sockaddr *)&this->_address,
                  (socklen_t *)&this->_addrLen);
  if (fd < 0) {
    std::cout << RED "Accept failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << GRN "New client accepted" NC << std::endl;
  pollfd newPoll;
  newPoll.fd = fd;
  newPoll.events = POLLIN | POLLHUP | POLLRDHUP;
  newPoll.revents = 0;
  this->_polls.push_back(newPoll);

  char hostName[NI_MAXHOST];
  char hostService[NI_MAXSERV];
  memset(hostName, 0, NI_MAXHOST);
  memset(hostService, 0, NI_MAXSERV);

  int res = getnameinfo((struct sockaddr *)&this->_address, this->_addrLen,
                        hostName, NI_MAXHOST, hostService, NI_MAXSERV, 0);
  if (res) {
    std::cout << RED "Getnameinfo failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << GRN "New client connected on port " << hostService << NC
            << std::endl;

  if (this->initChecker(fd) == -1) {
    close(fd);
  }

  User *newUser;
  newUser = new User(fd, hostName, hostService);
  this->_users.insert(std::make_pair(fd, newUser));

  askUserData(fd);
  // User newUser(fd, hostName, hostService);
  // this->_users.push_back(newUser);
}

int Server::initChecker(int fd) {
  char buffer[1000];

  send(fd, "Enter password : ", 17, 0);
  ssize_t bytes_received = recv(fd, buffer, 1000, 0);
  if (bytes_received < 0) {
    std::cout << RED "Recv failed" NC << std::endl;
    close(fd);
    return -1;
  }

  for (ssize_t i = 0; i < bytes_received; ++i) {
    if (buffer[i] == '\n' || buffer[i] == '\r') {
      buffer[i] = '\0';
      break;
    }
  }

  if (strcmp(buffer, this->_password.c_str()) == 0) {
    std::cout << GRN "Password correct" NC << std::endl;
    return 0;
  } else {
    std::cout << RED "Password incorrect" NC << std::endl;
    send(fd, "Password incorrect.\n", 20, 0);
  }
  return -1;
}

int Server::createChannel(std::string channelName, User *u) {
  Channel *newChannel = new Channel(channelName);
  _channels.insert(std::make_pair(channelName, newChannel));
  this->joinChannel(channelName, u);

  return 0;
}

int Server::joinChannel(std::string channelName, User *u) {
  std::map<std::string, Channel *>::iterator it = _channels.find(channelName);
  if (it != _channels.end()) {
    it->second->addUser(u);
    return 0;
  } else
    return -1;
}

std::map<std::string, Channel *> &Server::getChannel() { return _channels; }
