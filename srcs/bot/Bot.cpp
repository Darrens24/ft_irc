#include "../../headers/Bot.hpp"
#include <sys/socket.h>

int main(int ac, char **av) { Bot return 0; }

/***\ CONSTRUCTORS \***/

Bot::Bot(Server *server) : _server(server) {
  this->_botMessages.push_back("Bienvenue a tout le monde sur le serveur !");
  this->_botMessages.push_back("Je suis un bot !");
  this->_nickname = "Sunbot";
  this->_username = "Sunbot";
  this->_realname = "elbot";
  this->_botPassword = server->getPassword();
  this->_botPort = server->getPort();
}

Bot::~Bot() {}

Bot::Bot(const Bot &src) { *this = src; }

Bot &Bot::operator=(const Bot &rhs) {
  if (this != &rhs) {
    this->_server = rhs._server;
    this->_nickname = rhs._nickname;
    this->_username = rhs._username;
    this->_realname = rhs._realname;
    this->_botPort = rhs._botPort;
    this->_botPassword = rhs._botPassword;
    this->_botMessages = rhs._botMessages;
  }
  return *this;
}

/***\ METHODS \***/

bool Bot::connectToServer() {
  this->_botSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->_botSocket == -1) {
    std::cerr << "Error: socket creation failed" << std::endl;
    return false;
  }

  if (connect(this->_botSocket, (struct sockaddr *)&this->_server->getAddr(),
              sizeof(this->_server->getAddr())) == -1) {
    std::cerr << "Error: connection failed" << std::endl;
    return false;
  }

  std::string password = "PASS " + this->_botPassword + "\n";
  std::string nickname = "NICK " + this->_nickname + "\n";
  std::string username = "USER " + this->_username + " 0 * " + this->_realname;
  std::string everything = password + nickname + username;

  this->response(everything);
  return true;
}

void Bot::response(std::string message) {
  std::string response = message + "\r\n";
  send(this->_botSocket, response.c_str(), response.length(), 0);
}
