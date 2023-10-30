#pragma once

#define ERR_NEEDMOREPARAMS(client1, command1)                                  \
  "461 " + client1 + " " + command1 +                                          \
      " :" RED "Error" WHT ": Not enough parameters" NC

#define ERR_ALREADYREGISTERED(client1)                                         \
  "462 " + client1 + " :" RED "Error" WHT ": You may not reregister" NC

#define ERR_PASSWDISMATCH(client1)                                             \
  "464 " + client1 + " :" RED "Error" WHT ": Password incorrect" NC

#define ERR_NONICKNAMEGIVEN(client1)                                           \
  "431 " + client1 + " :" RED " Error" WHT ": No nickname given" NC

#define ERR_ERRONEUSNICKNAME(client1, command1)                                \
  "432 " + client1 + " " + command1 +                                          \
      " :" RED "Error" WHT ": Erroneus nickname" NC

#define ERR_NICKNAMEINUSE(client1, command1)                                   \
  "433 " + client1 + " " + command1 +                                          \
      " :" RED "Error" WHT ": Nickname is already in use" NC

#define ERR_BADCHANNELKEY(client1, channel1)                                   \
  "475 " + client1 + " " + channel1 +                                          \
      " :" RED "Error" WHT ": Cannot join channel (+k)" NC

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
