#pragma once

#include "Server.hpp"
#include <numeric>
#include <string>

class Server;

class Command {
private:
protected:
  Server *_srv;
  // bool    _auth;
  Command();
  Command(const Command &src);

public:
  Command(Server *srv);
  virtual ~Command();

  // bool auth_required() const;

  virtual bool execute(User *client, std::vector<std::string> args) = 0;
};

// class Join : public Command {
// public:
//   Join(Server *srv);
//   ~Join();
//
//   void execute(User *client, std::vector<std::string> args);
// };
//
class Usercmd : public Command {
public:
  Usercmd(Server *srv);
  ~Usercmd();

  bool execute(User *client, std::vector<std::string> args);
};

class Nick : public Command {
public:
  Nick(Server *srv);
  ~Nick();

  bool execute(User *client, std::vector<std::string> args);
};

class Pass : public Command {
public:
  Pass(Server *srv);
  ~Pass();

  bool execute(User *client, std::vector<std::string> args);
};
