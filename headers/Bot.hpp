#pragma once

#include "../headers/Server.hpp"

class Bot {
public:
  Bot(Server *server);
  ~Bot();
  Bot(const Bot &src);
  Bot &operator=(const Bot &rhs);

  /***\ GETTERS \***/
  Server *getServer() const { return _server; }
  int getBotSocket() const { return _botSocket; }
  int getBotPort() const { return _botPort; }
  std::string getNickname() const { return _nickname; }
  std::string getUsername() const { return _username; }
  std::string getRealname() const { return _realname; }
  std::string getBotPassword() const { return _botPassword; }
  std::vector<std::string> getBotMessages() const { return _botMessages; }

  /***\ METHODS \***/
  bool connectToServer();
  void response(std::string message);

private:
  Server *_server;
  int _botSocket;
  int _botPort;
  std::string _nickname;
  std::string _username;
  std::string _realname;
  std::string _botPassword;
  std::vector<std::string> _botMessages;
};
