#pragma once

#include "Server.hpp"
#include <numeric>
#include <string>

class Server;

class Command {
protected:
  Server *_srv;
  Command();
  Command(const Command &src);

public:
  Command(Server *srv);
  virtual ~Command();

  std::string trim(const std::string &str);
  virtual bool execute(User *client, std::vector<std::string> args) = 0;
};

class Join : public Command {
public:
  Join(Server *srv);
  ~Join();

  bool execute(User *client, std::vector<std::string> args);
};

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

class Privmsg : public Command {
public:
  Privmsg(Server *srv);
  ~Privmsg();

  bool execute(User *client, std::vector<std::string> args);
  void SendPrivateMessage(User *client, std::vector<std::string> args);
};

class Kick : public Command {
public:
  Kick(Server *srv);
  ~Kick();

  bool execute(User *client, std::vector<std::string> args);
};

class Invite : public Command {
public:
  Invite(Server *srv);
  ~Invite();

  bool execute(User *client, std::vector<std::string> args);
};
