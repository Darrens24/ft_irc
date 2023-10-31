#pragma once

#define ERR_NEEDMOREPARAMS(client1, command1)                                  \
  "461 " + client1 + " " + command1 +                                          \
      " :" RED "Error" WHT ": Wrong number of parameters" NC

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

#define ERR_CHANNELISFULL(client1, channel1)                                   \
  "471 " + client1 + " " + channel1 + " :" WHT "Channel is full" NC

#define RPL_NAMREPLY(client1, symbol1, channel1, nick1)                        \
  "353 " + client1 + " " + symbol1 + " " + channel1 + " :" + nick1

#define RPL_ENDOFNAMES(client1, channel1)                                      \
  "366 " + client1 + " " + channel1 + " :" WHT "End of /NAMES list" NC

#define RPL_TOPIC(client1, channel1)                                           \
  "332 " + client1 + " " + channel1 + " :" WHT "topics" NC

#define ERR_NOSUCHCHANNEL(client1, channel1)                                   \
  "403 " + client1 + " " + channel1 + " :" WHT "No such channel" NC

#define ERR_INVITEONLYCHAN(client1, channel1)                                  \
  "473 " + client1 + " " + channel1 + " :" WHT "This channel is invite only" NC

#define ERR_BADCHANMASK(client1, channel1)                                     \
  "476 " + client1 + " " + channel1 + " :" WHT "Incorrect format" NC

#define ERR_NOSUCHNICK(client1, channel1)                                      \
  "401 " + client1 + " " + channel1 + " :" WHT "No such nick" NC

#define ERR_INVALIDKEY(client1, channel1)                                      \
  "525 " + client1 + " " + channel1 +                                          \
      " :" RED "Error" WHT ": Bad params" NC

#define ERR_NORECIPIENT(client1, command1)                                     \
  "411 " + client1 + " " + command1 +                                          \
      " :" RED "Error" WHT ": No recipient given (" + command1 + ")" NC

#define ERR_NOTONCHANNEL(client1, channel1)                                    \
  "442 " + client1 + " " + channel1 + " :" WHT "You're not on that channel" NC

#define ERR_CHANOPRIVSNEEDED(client1, channel1)                                \
  "482 " + client1 + " " + channel1 + " :" WHT "You're not channel operator" NC

#define ERR_USERONCHANNEL(client1, nickname1, channel1)                        \
  "443 " + client1 + " " + nickname1 + " " + channel1 +                        \
      " :" WHT "is already on channel" NC

#define ERR_UNKNOWNMODE(client1, mode1)                                        \
  "472 " + client1 + " " + mode1 + " :" WHT "is unknown mode char to me" NC

#define RPL_INVITING(client1, nickname1, channel1)                             \
  "341 " + client1 + " " + nickname1 + " " + channel1 +                        \
      " :" WHT "has been invited to channel" NC

#define RPL_UMODEIS(client1, mode1)                                            \
  "221 " + client1 + " " + mode1 + " :" WHT "is your current mode" NC

#define RPL_CHANNELMODEIS(client1, channel1, mode1)                            \
  "324 " + client1 + " " + channel1 + " " + mode1 +                            \
      " :" WHT "is current channel mode" NC
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
  void SendPrivateMessage(User *client,std::string target, std::vector<std::string> args);
  bool execute_msg(User *client,std::string target, std::vector<std::string> args);
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

class Topic : public Command {
public:
  Topic(Server *srv);
  ~Topic();

  bool execute(User *client, std::vector<std::string> args);
};

class Mode : public Command {
public:
  Mode(Server *srv);
  ~Mode();

  bool execute(User *client, std::vector<std::string> args);
  bool execute_differents_modes(User *client, std::vector<std::string> args,
                                Channel *tmpChan);
};
