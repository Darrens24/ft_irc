/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 14:57:54 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/27 16:49:54 by feliciencat      ###   ########.fr       */
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
  std::cout << MAG SERVERSPEAK YEL ": Socket created" NC << std::endl;

  if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &this->_opt,
                 sizeof(int))) {
    std::cout << RED "Setsockopt failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << MAG SERVERSPEAK YEL ": Socket reusable" NC << std::endl;

  if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0) {
    std::cout << RED "Fcntl failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << MAG SERVERSPEAK YEL ": Socket non-blocking" NC << std::endl;

  this->_address.sin_family = AF_INET;
  this->_address.sin_addr.s_addr = INADDR_ANY;
  this->_address.sin_port = htons(this->_port);
  this->_addrLen = sizeof(this->_address);

  if (bind(this->_socket, (struct sockaddr *)&this->_address,
           sizeof(this->_address)) < 0) {
    std::cout << RED "Bind failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << MAG SERVERSPEAK YEL ": Socket binded" NC << std::endl;

  if (listen(this->_socket, this->_maxClients) < 0) {
    std::cout << RED "Listen failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << MAG SERVERSPEAK YEL ": Server listening on port " << port << NC
            << std::endl;
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
  std::cout << MAG SERVERSPEAK YEL ": Poll server created" NC << std::endl;

  while (1) {
    int pollCount = poll(&this->_polls[0], this->_polls.size(), -1);
    if (pollCount < 0) {
      std::cout << RED "Poll failed" NC << std::endl;
      exit(EXIT_FAILURE);
    }

    for (long unsigned int i = 0; i < this->_polls.size(); i++) {
      if (this->_polls[i].revents & POLLRDHUP) {
        std::cout << RED CLIENTSPEAK << " " << this->_polls[i].fd
                  << ": disconnected" NC << std::endl;
        close(this->_polls[i].fd);
        this->_users.erase(this->_polls[i].fd);
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
  memset(buffer, 0, 1024);

  ssize_t read = recv(fd, buffer, 1024, 0);
  std::cout << BLU CLIENTSPEAK << " " << this->_polls[i].fd << W << ": "
            << buffer << NC;
  if (read < 0) {
    std::cout << RED "Recv failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  if (read == 0) {
    std::cout << RED "Couldn't read from client" NC << std::endl;
    close(this->_polls[i].fd);
    this->_users.erase(this->_polls[i].fd);
    this->_polls.erase(this->_polls.begin() + i);
    return;
  }
  if (this->_users[fd]->getUserRegistered() == false ||
      this->_users[fd]->getRegistered() == false ||
      this->_users[fd]->getNickRegistered() == false)
    getBasicInfo(fd, buffer);
  else
    launchParser(buffer, fd);
}

bool Server::getBasicInfo(int fd, char buffer[1024]) {

  std::string str(buffer);
  std::vector<std::string> array = mySplit(str, "\r\n\t\v ");

  if (array.size() == 0) {
    return false;
  }
  if (array[0] == "CAP") {
    array.erase(array.begin());
    array.erase(array.begin());
  }
  if (array[0] == "PASS") {
    Pass pass(this);
    if (!pass.execute(this->_users[fd], array)) {
      return false;
    }
    array.erase(array.begin());
    array.erase(array.begin());
  }

  if (array[0] == "NICK") {
    if (this->_users[fd]->getRegistered() == true) {
      Nick nick(this);
      if (!nick.execute(this->_users[fd], array)) {
        return false;
      }
      array.erase(array.begin());
      array.erase(array.begin());
    } else {
      this->_users[fd]->response(RED "You need to set password first" NC);
    }
  }
  if (array[0] == "USER") {
    if (this->_users[fd]->getRegistered() == true) {
      Usercmd user(this);
      if (!user.execute(this->_users[fd], array)) {
        return false;
      }
      array.erase(array.begin());
      array.erase(array.begin());
    } else {
      this->_users[fd]->response(RED "You need to set password first" NC);
    }
  }
  if (!this->_users[fd]->getNickname().empty() &&
      this->_users[fd]->getUserRegistered() == true &&
      this->_users[fd]->getRegistered() == true) {
    this->_users[fd]->response("CAP * LS :multi-prefix sasl");
    std::string welcomeMssg = "001 " + this->_users[fd]->getNickname() +
                              " :Welcome to the Internet Relay Network";
    this->_users[fd]->response(welcomeMssg);
  }
  return true;
}

void Server::launchParser(char buffer[1024], int fd) {
  std::string str(buffer);
  (void)fd;
  std::vector<std::string> array = mySplit(str, "\r\n\t\v ");

  if (array[0] == "PASS") {
    Pass pass(this);
    if (!pass.execute(this->_users[fd], array)) {
      return;
    }
    array.erase(array.begin());
    array.erase(array.begin());
  }
  if (array[0] == "NICK") {
    if (this->_users[fd]->getRegistered() == true) {
      Nick nick(this);
      if (!nick.execute(this->_users[fd], array)) {
        return;
      }
      array.erase(array.begin());
      array.erase(array.begin());
    } else {
      this->_users[fd]->response(RED "You need to set password first" NC);
    }
  }
  if (array[0] == "USER") {
    if (this->_users[fd]->getRegistered() == true) {
      Usercmd user(this);
      if (!user.execute(this->_users[fd], array)) {
        return;
      }
      array.erase(array.begin());
      array.erase(array.begin());
    } else {
      this->_users[fd]->response(RED "You need to set password first" NC);
    }
  }
  if (array[0] == "JOIN") {
    Join join(this);
    join.execute(this->_users[fd], array);
    if (this->_users.find(fd) != this->_users.end())
      std::cout << "User found" << std::endl;
  }
  if (array[0] == "PRIVMSG") {
    Privmsg privmsg(this);
    privmsg.execute(this->_users[fd], array);
  }
  if (array[0] == "KICK") {
    Kick kick(this);
    kick.execute(this->_users[fd], array);
  }
  if (array[0] == "INVITE") {
    Invite invite(this);
    invite.execute(this->_users[fd], array);
  }
}

void Server::acceptNewClient() {
  int fd = accept(this->_socket, (struct sockaddr *)&this->_address,
                  (socklen_t *)&this->_addrLen);
  if (fd < 0) {
    std::cout << RED "Accept failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << GRN CLIENTSPEAK " " << fd << W ": New client accepted" NC
            << std::endl;
  pollfd newPoll;
  newPoll.fd = fd;
  newPoll.events = POLLIN | POLLHUP | POLLHUP;
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
  std::cout << GRN CLIENTSPEAK " " << fd << W ": New client connected on port "
            << hostService << NC << std::endl;

  User *newUser;
  newUser = new User(fd, hostName, hostService, this->_password);
  this->_users.insert(std::make_pair(fd, newUser));
}

/***\ channels \***/

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

std::string sendMsgToChannel(std::string target, std::string msg, User *u);

std::map<std::string, Channel *> &Server::getChannel() { return _channels; }

Channel *Server::getChannelByName(std::string name) {
  std::cout << name << std::endl;
  if (_channels[name] != NULL) {
    return _channels[name];
  } else {
    return NULL;
  }
}

User *Server::getUserByNickname(std::string nickname) {
  for (std::map<int, User *>::iterator it = _users.begin(); it != _users.end();
       ++it) {
    if (it->second->getNickname() == nickname) {
      return it->second;
    }
  }
  return NULL;
}

bool Server::isNicknameAvailable(std::string username) {
  for (std::map<int, User *>::iterator it = _users.begin(); it != _users.end();
       ++it) {
    if (it->second->getNickname() == username)
      return false;
  }
  return true;
}

std::vector<User *> Server::getUsersOnly() {
  std::vector<User *> usersOnly;

  for (std::map<int, User *>::iterator it = _users.begin(); it != _users.end();
       ++it) {
    usersOnly.push_back(it->second);
  }

  return usersOnly;
}
