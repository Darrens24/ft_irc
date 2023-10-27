/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 10:43:37 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/26 15:43:09 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"

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

Command::Command(Server *srv) : _srv(srv) {}

Command::~Command() {}

/*
 *            PASS COMMAND
 * */

Pass::Pass(Server *srv) : Command(srv) {}

Pass::~Pass(){};

bool Pass::execute(User *client, std::vector<std::string> args) {
  if (args.size() < 2) {
    client->response(ERR_NEEDMOREPARAMS(client->getHostname(), "PASS"));
    return false;
  }

  if (client->getRegistered()) {
    client->response(ERR_ALREADYREGISTERED(client->getHostname()));
    return false;
  }

  if (args[1].compare(client->getPasswd())) {
    client->response(ERR_PASSWDISMATCH(client->getHostname()));
    return false;
  }

  std::string login = GRN "<" + client->getHostname() +
                      "> " W
                      ":Login succesful, please now enter your username and "
                      "nickname with USER [Username] and NICK [Nickname]" NC;
  client->setRegistered();
  client->response(login);
  return true;
}

/*
 *            NICK COMMAND
 * */

Nick::Nick(Server *srv) : Command(srv) {}

Nick::~Nick(){};

bool Nick::execute(User *client, std::vector<std::string> args) {
  if (args.size() < 2) {
    client->response(ERR_NONICKNAMEGIVEN(client->getHostname()));
    return false;
  }

  if (args[1].find(":") != std::string::npos ||
      args[1].find("#") != std::string::npos) {
    client->response(ERR_ERRONEUSNICKNAME(client->getHostname(), "NICK"));
    return false;
  }

  if (this->_srv->isNicknameAvailable(args[1]) == false) {
    client->response(ERR_NICKNAMEINUSE(client->getHostname(), "NICK"));
    return false;
  }

  std::string login =
      GRN "<" + client->getHostname() + "> " W ":Nickname has been set" NC;
  client->setNickname(args[1]);
  client->response(login);
  return true;
}

/*
 *            USER COMMAND
 * */

Usercmd::Usercmd(Server *srv) : Command(srv) {}

Usercmd::~Usercmd(){};

bool Usercmd::execute(User *client, std::vector<std::string> args) {
  if (args.size() < 5) {
    client->response(ERR_NEEDMOREPARAMS(client->getHostname(), "USER"));
    return false;
  }

  if (client->getUserRegistered()) {
    client->response(ERR_ALREADYREGISTERED(client->getHostname()));
    return false;
  }

  std::string login =
      GRN "<" + client->getHostname() + "> " W ":User has been set" NC;
  client->setUsername(args[1]);
  client->setRealName(args[4]);
  client->setUserRegistered();
  client->response(login);
  return true;
}
