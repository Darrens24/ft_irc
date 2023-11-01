/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 14:57:54 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/30 19:45:50 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

/***\ CONSTRUCTORS \***/

Server::Server(int port, std::string password)
    : _serverName("our-IRC"), _password(password), _port(port), _opt(1),
      _maxClients(30) {

  this->_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->_socket == -1) {
    std::cout << RED SERVERSPEAK " Error: Socket creation failed" NC
              << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << MAG SERVERSPEAK NC ": Socket created" << std::endl;

  if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &this->_opt,
                 sizeof(int))) {
    std::cout << RED SERVERSPEAK " Error: Setsockopt failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << MAG SERVERSPEAK NC ": Socket reusable" << std::endl;

  if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0) {
    std::cout << RED SERVERSPEAK " Error: Fcntl failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << MAG SERVERSPEAK NC ": Socket set non-blocking" << std::endl;

  this->_address.sin_family = AF_INET;
  this->_address.sin_addr.s_addr = INADDR_ANY;
  this->_address.sin_port = htons(this->_port);
  this->_addrLen = sizeof(this->_address);

  if (bind(this->_socket, (struct sockaddr *)&this->_address,
           sizeof(this->_address)) < 0) {
    std::cout << RED SERVERSPEAK " Error: Bind failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << MAG SERVERSPEAK NC ": Socket binded" << std::endl;

  if (listen(this->_socket, this->_maxClients) < 0) {
    std::cout << RED SERVERSPEAK " Error: Listen failed" NC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << MAG SERVERSPEAK NC ": Ready! Server listening on port " << port
            << std::endl;
}

Server::~Server() {
  while (!this->_polls.empty()) {
    this->_polls.pop_back();
  }
  while (!this->_users.empty()) {
    delete this->_users.begin()->second;
    this->_users.erase(this->_users.begin());
  }
  while (!this->_channels.empty()) {
    delete this->_channels.begin()->second;
    this->_channels.erase(this->_channels.begin());
  }
}

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

/***\ COMMANDS \***/

void Server::removeUserFromChannelDB(int i) {
  for (std::map<std::string, Channel *>::iterator it = this->_channels.begin();
       it != this->_channels.end(); ++it) {
    if (it->second->isInChannel(this->_users[this->_polls[i].fd])) {
      this->_channels[it->first]->removeUserFromChannel(
          this->_users[this->_polls[i].fd]);
    }
    if (it->second->getUsersOfChannel().size() == 0) {
      delete it->second;
      this->_channels.erase(it);
      if (this->_channels.size() == 0)
        break;
    }
  }
}

void Server::removeUserFromServer(int i) {
  this->removeUserFromChannelDB(i);
  close(this->_polls[i].fd);
  this->_users[this->_polls[i].fd]->setUserUnregistered();
  this->_users[this->_polls[i].fd]->setNickUnregistered();
  delete this->_users[this->_polls[i].fd];
  this->_users.erase(this->_polls[i].fd);
  this->_polls.erase(this->_polls.begin() + i);
}

/***\ INIT \***/

void Server::start() {
  pollfd serverPoll;
  serverPoll.fd = this->_socket;
  serverPoll.events = POLLIN | POLLHUP | POLLRDHUP;
  serverPoll.revents = 0;

  this->_polls.push_back(serverPoll);
  std::cout << MAG SERVERSPEAK NC ": Poll server created" << std::endl;

  while (server_up) {
    int pollCount = poll(&this->_polls[0], this->_polls.size(), -1);
    if (pollCount < 0 && server_up) {
      std::cout << RED SERVERSPEAK " Error: Poll failed" NC << std::endl;
      exit(EXIT_FAILURE);
    }

    for (long unsigned int i = 0; i < this->_polls.size(); i++) {
      if (this->_polls[i].revents & POLLRDHUP) {
        std::cout << RED CLIENTSPEAK(this->_userport[this->_polls[i].fd])
                  << ": Disconnected" NC << std::endl;
        this->removeUserFromServer(i);
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
  static std::string save;
  (void)i;

  ssize_t read = recv(fd, buffer, 1024, 0);
  if (read < 0) {
    std::cout << RED SERVERSPEAK " Error: Recv failed" NC << std::endl;
    return;
  }

  std::string strbuffer = buffer;
  if (save.size() > 0)
    strbuffer = save + buffer;
  std::string tmp = std::string(buffer);
  if (tmp.find('\n') != tmp.npos) {
    std::cout << BLU CLIENTSPEAK(this->_userport[fd]) << NC << ": "
              << strbuffer;
    if (this->_users[fd]->getUserRegistered() == false ||
        this->_users[fd]->getRegistered() == false ||
        this->_users[fd]->getNickname() == "")
      getBasicInfo(fd, strbuffer);
    else
      launchParser(strbuffer, fd);
    save = "";
  } else
    save = strbuffer;
}

bool Server::getBasicInfo(int fd, std::string str) {

  std::vector<std::string> array = mySplit(str, "\r\n\t\v ");

  if (array.size() == 0) {
    return false;
  }

  if (array[0] == "CAP") {
    array.erase(array.begin());
    if (array.size() > 0)
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
  if (this->_users[fd]->getNickRegistered() == true &&
      this->_users[fd]->getUserRegistered() == true &&
      this->_users[fd]->getRegistered() == true) {
    std ::cout << "we are in" << std::endl;
    this->_users[fd]->response("CAP * LS :multi-prefix sasl");
    std::string welcomeMssg = "001 " + this->_users[fd]->getNickname() +
                              " :Welcome to our Internet Relay Network!";
    this->_users[fd]->response(welcomeMssg);
  }
  return true;
}

void Server::launchParser(std::string str, int fd) {
  std::vector<std::string> array = mySplit(str, "\r\n\t\v ");

  if (array.size() == 0)
    return;

  if (array[0] == "PASS") {
    Pass pass(this);
    if (!pass.execute(this->_users[fd], array))
      return;
  }
  if (array[0] == "NICK") {
    if (this->_users[fd]->getRegistered() == true) {
      Nick nick(this);
      if (!nick.execute(this->_users[fd], array))
        return;
    } else {
      this->_users[fd]->response(RED "You need to set password first" NC);
    }
  }
  if (array[0] == "USER") {
    if (this->_users[fd]->getRegistered() == true) {
      Usercmd user(this);
      if (!user.execute(this->_users[fd], array))
        return;
    } else {
      this->_users[fd]->response(RED "You need to set password first" NC);
    }
  }
  if (array[0] == "JOIN") {
    Join join(this);
    join.execute(this->_users[fd], array);
  }
  if (array[0] == "PART") {
    Part part(this);
    part.execute(this->_users[fd], array);
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
  if (array[0] == "TOPIC") {
    Topic topic(this);
    topic.execute(this->_users[fd], array);
  }
  if (array[0] == "MODE") {
    Mode mode(this);
    mode.execute(this->_users[fd], array);
  }
}

void Server::acceptNewClient() {
  int fd = accept(this->_socket, (struct sockaddr *)&this->_address,
                  (socklen_t *)&this->_addrLen);
  if (fd < 0) {
    std::cout << RED SERVERSPEAK " Error: Accept failed" NC << std::endl;
    return;
  }
  std::cout << GRN SERVERSPEAK << NC ": New client accepted" << std::endl;

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
    std::cout << RED SERVERSPEAK " Error: Getnameinfo failed" NC << std::endl;
    return;
  }

  this->_userport[fd] = hostService;
  std::cout << GRN CLIENTSPEAK(this->_userport[fd])
            << NC ": New client connected on port " GRN << hostService << NC
            << std::endl;

  User *newUser;
  newUser = new User(fd, hostName, hostService, this->_password);
  this->_users.insert(std::make_pair(fd, newUser));
}

/***\ getters \***/

Channel *Server::getChannelByName(std::string name) {
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

std::vector<User *> Server::getUsersOnly() {
  std::vector<User *> usersOnly;

  for (std::map<int, User *>::iterator it = _users.begin(); it != _users.end();
       ++it) {
    usersOnly.push_back(it->second);
  }

  return usersOnly;
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

/***\ bools \***/

bool Server::isNicknameAvailable(std::string username) {
  for (std::map<int, User *>::iterator it = _users.begin(); it != _users.end();
       ++it) {
    if (it->second->getNickname() == username)
      return false;
  }
  return true;
}
