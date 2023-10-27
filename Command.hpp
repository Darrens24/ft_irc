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

  virtual void execute(User *client, std::vector<std::string> args) = 0;
};

class Join : public Command {
public:
  Join(Server *srv);
  ~Join();

  void execute(User *client, std::vector<std::string> args);
};
<<<<<<< HEAD
=======

class Usercmd : public Command {
public:
  Usercmd(Server *srv);
  ~Usercmd();

  void execute(User *client, std::vector<std::string> args);
};

class Nick : public Command {
public:
  Nick(Server *srv);
  ~Nick();

  void execute(User *client, std::vector<std::string> args);
};

class Pass : public Command {
public:
  Pass(Server *srv);
  ~Pass();

  void execute(User *client, std::vector<std::string> args);
};
>>>>>>> pedro
