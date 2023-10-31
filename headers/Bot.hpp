#pragma once

#define RED "\033[1;31m" // FOR DESTRUCTORS
#define GRN "\033[1;32m" // FOR CONSTRUCTORS
#define YEL "\033[1;33m" // FOR ERRORS
#define BLU "\033[1;34m"
#define MAG "\033[1;35m"
#define CYA "\033[1;36m"
#define W "\033[1;97m"
#define WHT "\033[1;97m"
#define NC "\033[1;0m"
#define I "\033[1;3m"
#define U "\033[1;4m"

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

class Bot {
public:
  Bot(int port, std::string password);
  ~Bot();
  Bot(const Bot &src);
  Bot &operator=(const Bot &rhs);

  /***\ GETTERS \***/
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
  int _botSocket;
  int _botPort;
  std::string _nickname;
  std::string _username;
  std::string _realname;
  std::string _botPassword;
  std::vector<std::string> _botMessages;
};
