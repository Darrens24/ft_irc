/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 19:59:54 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/30 16:07:50 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

Mode::Mode(Server *srv) : Command(srv) {}

Mode::~Mode() {}

// MODE <#channel> [+|-]|o|i|t|k|l]

std::vector<std::string> myModeSplit(std::string str, std::string sep) {
  char *cstr = const_cast<char *>(str.c_str());
  char *current;
  std::vector<std::string> arr;
  current = strtok(cstr, sep.c_str());
  while (current != NULL) {
    arr.push_back(current);
    current = strtok(NULL, sep.c_str());
  }
  return arr;
}

bool Mode::execute_differents_modes(User *client, std::vector<std::string> args,
                                    Channel *tmpChan) {

  bool diff_mode = false;
  if ((args[2][0] == '+' || args[2][0] == '-') && args[2].length() > 1) {
    if (args[2][0] == '+')
      diff_mode = true;
    args[2].erase(0, 1);
  } else {
    client->response(ERR_NEEDMOREPARAMS(client->getNickname(), args[1]));
    return false;
  }
  while (args[2].length() > 0) {
    if (args[2][0] == 'i') {
      if (diff_mode == false) {
        if (tmpChan->findMode('i') == true) {
          tmpChan->removeMode('i');
          send(client->getFd(), "Channel is no longer invite only\r\n", 34, 0);
        } else {
          send(client->getFd(),
               "invite only can't be deleted because fonction is not "
               "activated\n",
               64, 0);
          return false;
        }
      } else {
        if (tmpChan->findMode('i') == true) {
          send(client->getFd(), "Channel is already invite only\n", 32, 0);
          return false;
        } else {
          tmpChan->addMode('i');
          send(client->getFd(), "Channel is now invite only\n", 28, 0);
        }
      }
    }
    if (args[2][0] == 't') {
      if (diff_mode == false) {
        if (tmpChan->findMode('t') == true) {
          tmpChan->removeMode('t');
          send(client->getFd(), "all users can now change the topic\n", 36, 0);
        } else {
          send(client->getFd(),
               "topic can't be deleted because fonction is not activated\n", 59,
               0);
          return false;
        }
      } else {
        if (tmpChan->findMode('t') == true) {
          send(client->getFd(), "topic is already locked\n", 25, 0);
          return false;
        } else {
          tmpChan->addMode('t');
          send(client->getFd(), "topic is now locked\n", 21, 0);
        }
      }
    }
    if (args[2][0] == 'k') {
      if (diff_mode == false) {
        if (tmpChan->findMode('k') == true) {
          tmpChan->removeMode('k');
          tmpChan->setKey("");
          send(client->getFd(), "unset password\n", 16, 0);
        } else {
          send(client->getFd(), "password already unset\n", 24, 0);
          return false;
        }
      } else {
        if (args.size() != 4) {
          std::cout << "construction : 'MODE <#channel> <mode> <key>' "
                    << std::endl;
          return false;
        }
        if (tmpChan->findMode('k') == true) {
          send(client->getFd(),
               "key already set , delete it if you want to change\n", 51, 0);
          return false;
        } else {
          if (args[3].length() > 0) {
            tmpChan->setKey(args[3]);
            tmpChan->addMode('k');
            send(client->getFd(), "set key\n", 9, 0);
          } else {
            client->response(ERR_INVALIDKEY(client->getNickname(),
                                            tmpChan->getChannelName()));
            return false;
          }
        }
      }
    }
    if (args[2][0] == 'o') {
      /*Users with this mode may perform channel moderation tasks such as
       kicking users, applying channel modes, and set other users to operator
       (or lower) status.*/
      if (args.size() != 4 || args[3].length() == 0) {
        std::cout << "construction : 'MODE <#channel> [+/-]o <target>' "
                  << std::endl;
        return false;
      }
      User *tmpUser = _srv->getUserByNickname(args[3]);
      if (tmpUser == NULL) {
        std::cout << "User doesn't exist" << std::endl;
        return false;
      }
      Channel *tmpChan = _srv->getChannelByName(args[1]);
      if (tmpChan == NULL) {
        std::cout << "Channel doesn't exist" << std::endl; // ERR_NOSUCHCHANNEL
        return false;
      }
      if (diff_mode == false) {
        if (tmpUser->isUserOperator(tmpChan)) {
          tmpUser->removeChannelWhereUserIsOperator(tmpChan);
          send(client->getFd(), "all users can now change the topic\n", 36, 0);
        } else {
          send(client->getFd(),
               "topic can't be deleted because fonction is not activated\n", 59,
               0);
          return false;
        }
      } else {
        if (tmpChan->findMode('o') == true) {
          send(client->getFd(), "topic is already locked\n", 25, 0);
          return false;
        } else {
          tmpChan->addMode('o');
          send(client->getFd(), "topic is now locked\n", 21, 0);
        }
      }
    }

    args[2].erase(0, 1);
  }
  return true;
}

bool Mode::execute(User *client, std::vector<std::string> args) {
  if (args.size() < 2) {
    std::cout << "wrong arguments" << std::endl; // ERR
    return false;
  }
  if (args[1][0] == '#') {
    args[1].erase(0, 1);
  } else {
    // std::cout << "construction : 'MODE <#channel> <mode>' " << std::endl;
    return false;
  }
  Channel *tmpChan = _srv->getChannelByName(args[1]);
  if (tmpChan == NULL) {
    std::cout << "Channel doesn't exist" << std::endl; // ERR_NOSUCHCHANNEL
    return false;
  }
  if (!tmpChan->isInChannel(client)) {
    std::cout << "You are not in this channel" << std::endl; // ERR_NOTONCHANNEL
    return false;
  }
  if (args.size() == 2) {
    std::string msg;
    if (tmpChan->getModeString().length() > 0)
      msg = "The modes of this channel are : " + tmpChan->getModeString();
    else
      msg = "This channel has no modes.";
    client->response(msg);
    return true;
  }
  if (args.size() > 2) {
    if (execute_differents_modes(client, args, tmpChan) == false)
      return false;
  }
  return true;
}
