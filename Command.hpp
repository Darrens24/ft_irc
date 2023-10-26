#pragma once

#include "Server.hpp"
#include <numeric>
#include <string>

class Server;

class Command {
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

class Privmsg : public Command {
  public:
    Privmsg(Server *srv);
    ~Privmsg();

    void execute(User *client, std::vector<std::string> args);
    void SendPrivateMessage(User *client, std::vector<std::string> args);
};
