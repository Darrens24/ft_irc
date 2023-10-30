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

#include "Command.hpp"
#include "Server.hpp"

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

  client->setNickname(args[1]);
  std::string login =
      GRN "<" + client->getNickname() + "> " W ":Nickname has been set" NC;
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

  client->setUsername(args[1]);
  client->setRealName(args[4]);
  client->setUserRegistered();
  std::string login =
      GRN "<" + client->getUsername() + "> " W ":User has been set" NC;
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
