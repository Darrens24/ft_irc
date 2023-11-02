/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 10:43:37 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/27 16:29:03 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"
#include "../../headers/Server.hpp"

Command::Command(Server *srv) : _srv(srv) {}

Command::~Command() {}

/***\ PASS \***/

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

  std::string login =
      GRN "<" + client->getHostname() +
      "> " NC ":Login succesful, finish registration with USER and NICK ";
  client->setRegistered();
  client->response(login);
  return true;
}

/***\ NICK \***/

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

  client->setNickname(args[1]);
  client->setNickRegistered();
  std::string login =
      GRN "<" + client->getNickname() + "> " NC ":Nickname has been set";
  client->response(login);
  return true;
}

/***\ USER \***/

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

  client->setUsername(args[1]);
  client->setRealName(args[4]);
  client->setUserRegistered();
  std::string login =
      GRN "<" + client->getUsername() + "> " NC ":User has been set";
  client->response(login);
  return true;
}

std::string Command::trim(const std::string &str) {
  size_t first = str.find_first_not_of(' ');
  if (std::string::npos == first) {
    return str;
  }
  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last - first + 1));
}
