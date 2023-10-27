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

<<<<<<< HEAD
Command::Command(Server *srv) : _srv(srv) {}

Command::~Command() {}
=======
#define ERR_NEEDMOREPARAMS(client1, command1)                                  \
  RED "<" + client1 + "> <" + command1 +                                       \
      "> " WHT ":Not enough parameters (461)" NC

#define ERR_ALREADYREGISTERED(client1)                                         \
  RED "<" + client1 + "> " WHT ":You may not reregister(462)" NC

#define ERR_PASSWDISMATCH(client1)                                             \
  RED "<" + client1 + "> " WHT ":Password incorrect (464)" NC

#define ERR_NONICKNAMEGIVEN(client1)                                           \
  RED "<" + client1 + "> " WHT ":No nickname given (431)" NC

#define ERR_ERRONEUSNICKNAME(client1, command1)                                \
  RED "<" + client1 + "> <" + command1 + "> " WHT ":Erroneus nickname (432)" NC

#define ERR_NICKNAMEINUSE(client1, command1)                                   \
  RED "<" + client1 + "> <" + command1 +                                       \
      "> " WHT ":Nickname is already in use (433)" NC

#define ERR_NICKCOLLISION(client1, command1, user1, host1)                     \
  RED "<" + client1 + "> <" + command1 +                                       \
      "> " WHT ":Nickname collision KILL from <" + user1 + ">@<" + host1 +     \
      "> (436)" NC

Command::Command(Server *srv) : _srv(srv) {}

Command::~Command() {}

/*
 *            PASS COMMAND
 * */

Pass::Pass(Server *srv) : Command(srv) {}

Pass::~Pass(){};

void Pass::execute(User *client, std::vector<std::string> args) {
  if (args.empty()) {
    client->response(ERR_NEEDMOREPARAMS(client->getHostname(), "PASS"));
  }
  if (client->getRegistered())
    client->response(ERR_ALREADYREGISTERED(client->getHostname()));
  if (args[1].compare(client->getPasswd()))
    client->response(ERR_PASSWDISMATCH(client->getHostname()));
  std::string login = GRN "<" + client->getHostname() +
                      "> " W
                      ": Login succesful, please now enter your username and "
                      "nickname with USER [Username] and NICK [Nickname]" NC;
  client->response(login);
}

/*
 *            NICK COMMAND
 * */
//
// Nick::Nick(Server *srv) : Command(srv) {}
//
// Nick::~Nick(){};
//
// void Nick::execute(User *client, std::vector<std::string> args) {
//   if (args.empty()) {
//     client->response(ERR_NONICKNAMEGIVEN(client->getHostname()));
//   }
//   std::string login =
//       GRN "<" + client->getHostname() + "> : " W "Nickname has been set" NC;
//   client->response(login);
// }
//
// /*
//  *            USER COMMAND
//  * */
//
// Usercmd::Usercmd(Server *srv) : Command(srv) {}
//
// Usercmd::~Usercmd(){};
//
// void Usercmd::execute(User *client, std::vector<std::string> args) {
//   std::string login = "<" + client->getHostname() +
//                       "> : Login succesful, please now enter your username
//                       and " "nickname with USER [Username] and NICK
//                       [Nickname]";
//   client->response(login);
// }
>>>>>>> pedro
