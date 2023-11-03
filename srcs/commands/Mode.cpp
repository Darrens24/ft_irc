#include "../../headers/Command.hpp"
#include "../../headers/utils.hpp"

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

  if (client->isUserOperator(tmpChan) == false) {
    client->response(ERR_CHANOPRIVSNEEDED(client->getNickname(), args[1]));
    return false;
  }

  while (args[2].length() > 0) {
    if (args[2][0] == 'i') {
      if (diff_mode == false) {
        if (tmpChan->findMode('i') == true) {
          tmpChan->removeMode('i');
          std::string channeli = tmpChan->getChannelName();
          std::string modeChangeMessagei =
              ":" + client->getNickname() + "!~" + client->getUsername() +
              "@localhost MODE " + "#" + channeli + " -i";
          tmpChan->sentMessageToAllMembers(modeChangeMessagei);
        } else {
          client->response(
              "invite only can't be deleted because fonction is not activated");
          return false;
        }
      } else {
        if (tmpChan->findMode('i') == true) {
          client->response("invite only is already set");
          return false;
        } else {
          tmpChan->addMode('i');
          std::string channeli2 = tmpChan->getChannelName();
          std::string modeChangeMessagei2 =
              ":" + client->getNickname() + "!~" + client->getUsername() +
              "@localhost MODE " + "#" + channeli2 + " +i";
          tmpChan->sentMessageToAllMembers(modeChangeMessagei2);
        }
      }
    } else if (args[2][0] == 'l') {
      if (diff_mode == false) {
        if (tmpChan->findMode('l') == true) {
          tmpChan->removeMode('l');
          std::string channel_l = tmpChan->getChannelName();
          std::string modeChangeMessage_l =
              ":" + client->getNickname() + "!~" + client->getUsername() +
              "@localhost MODE " + "#" + channel_l + " -l";
          tmpChan->sentMessageToAllMembers(modeChangeMessage_l);
        } else {
          client->response(
              "limit can't be deleted because fonction is not activated");
          return false;
        }
      } else {
        if (args.size() != 4) {
          client->response(ERR_NEEDMOREPARAMS(client->getNickname(), args[1]));
          return false;
        }
        if (tmpChan->findMode('l') == true) {
          client->response("limit is already set");
          return false;
        } else {
          if (args[3].length() > 0) {
            int limit;
            try {
              limit = myStrAtoi(args[3]);
            } catch (std::exception &e) {
              client->response(ERR_INVALIDKEY(client->getNickname(),
                                              tmpChan->getChannelName()));
              return false;
            }
            if (limit < 1) {
              client->response(ERR_INVALIDKEY(client->getNickname(),
                                              tmpChan->getChannelName()));
              return false;
            }
            if ((unsigned int)tmpChan->getNumberofUsers() >=
                (unsigned int)limit) {
              client->response(ERR_CHANNELISFULL(client->getNickname(),
                                                 tmpChan->getChannelName()));
              return false;
            }
            tmpChan->setLimit(limit);
            tmpChan->addMode('l');
            std::string channel_l2 = tmpChan->getChannelName();
            std::string modeChangeMessage_l2 =
                ":" + client->getNickname() + "!~" + client->getUsername() +
                "@localhost MODE " + "#" + channel_l2 + " +l";
            tmpChan->sentMessageToAllMembers(modeChangeMessage_l2);
          } else {
            client->response(
                ERR_NEEDMOREPARAMS(client->getNickname(), args[1]));
            return false;
          }
        }
      }
    } else if (args[2][0] == 'k') {
      if (diff_mode == false) {
        if (tmpChan->findMode('k') == true) {
          tmpChan->removeMode('k');
          tmpChan->setKey("");
          std::string channel_k = tmpChan->getChannelName();
          std::string modeChangeMessage_k =
              ":" + client->getNickname() + "!~" + client->getUsername() +
              "@localhost MODE " + "#" + channel_k + " -k";
          tmpChan->sentMessageToAllMembers(modeChangeMessage_k);
        } else {
          client->response("password already unset");
          return false;
        }
      } else {
        if (args.size() != 4) {
          client->response(ERR_NEEDMOREPARAMS(client->getNickname(), args[1]));
          client->response("construction : 'MODE <#channel> <mode> <key>' ");
          return false;
        }
        if (tmpChan->findMode('k') == true) {
          client->response("password already set");
          return false;
        } else {
          if (args[3].length() > 0) {
            tmpChan->setKey(args[3]);
            tmpChan->addMode('k');
            std::string channel_k2 = tmpChan->getChannelName();
            std::string modeChangeMessage_k2 =
                ":" + client->getNickname() + "!~" + client->getUsername() +
                "@localhost MODE " + "#" + channel_k2 + " +k";
            tmpChan->sentMessageToAllMembers(modeChangeMessage_k2);
          } else {
            client->response(ERR_INVALIDKEY(client->getNickname(),
                                            tmpChan->getChannelName()));
            return false;
          }
        }
      }
    } else if (args[2][0] == 't') {
      if (diff_mode == false) {
        if (tmpChan->findMode('t') == true) {
          tmpChan->removeMode('t');
          send(client->getFd(), "all users can now change the topic\n", 36, 0);
          std::string channel_t = tmpChan->getChannelName();
          std::string modeChangeMessage_t =
              ":" + client->getNickname() + "!~" + client->getUsername() +
              "@localhost MODE " + "#" + channel_t + " -t";
          tmpChan->sentMessageToAllMembers(modeChangeMessage_t);
        } else {
          client->response("topic can't be deleted because fonction is not "
                           "activated");
          return false;
        }
      } else {
        if (tmpChan->findMode('t') == true) {
          client->response("topic is already set");
          return false;
        } else {
          tmpChan->addMode('t');
          std::string channel_t2 = tmpChan->getChannelName();
          std::string modeChangeMessage_t2 =
              ":" + client->getNickname() + "!~" + client->getUsername() +
              "@localhost MODE " + "#" + channel_t2 + " +t";
          tmpChan->sentMessageToAllMembers(modeChangeMessage_t2);
        }
      }
    } else if (args[2][0] == 'o') {
      if (args.size() != 4 || args[3].length() == 0) {
        client->response(ERR_NEEDMOREPARAMS(client->getNickname(), args[1]));
        client->response("construction : 'MODE <#channel> [+/-]o <target>' ");
        return false;
      }
      User *tmpUser = _srv->getUserByNickname(args[3]);
      if (tmpUser == NULL) {
        client->response(ERR_NOSUCHNICK(client->getNickname(), args[3]));
        return false;
      }
      Channel *FaketmpChan = _srv->getChannelByName(args[1]);
      if (FaketmpChan == NULL) {
        client->response(ERR_NOSUCHCHANNEL(client->getNickname(), args[1]));
        return false;
      }
      if (!FaketmpChan->isInChannel(tmpUser)) {
        client->response(ERR_NOTONCHANNEL(client->getNickname(), args[1]));
        return false;
      }
      if (diff_mode == false) {
        if (tmpUser->isUserOperator(FaketmpChan)) {
          tmpUser->removeChannelWhereUserIsOperator(FaketmpChan);
          FaketmpChan->removeOperator(tmpUser);
          std::string channel_o = tmpChan->getChannelName();
          std::string modeChangeMessage_o =
              ":" + client->getNickname() + "!~" + client->getUsername() +
              "@localhost MODE " + "#" + channel_o + " -o " +
              tmpUser->getNickname();
          tmpChan->sentMessageToAllMembers(modeChangeMessage_o);
        } else {
          client->response("user is not operator");
          return false;
        }
      } else {
        if (tmpUser->isUserOperator(FaketmpChan)) {
          client->response("user is already operator");
          return false;
        } else {
          tmpUser->addChannelWhereUserIsOperator(FaketmpChan);
          FaketmpChan->addOperator(tmpUser);
          std::string channel_o2 = tmpChan->getChannelName();
          std::string modeChangeMessage_o2 =
              ":" + client->getNickname() + "!~" + client->getUsername() +
              "@localhost MODE " + "#" + channel_o2 + " +o " +
              tmpUser->getNickname();
          tmpChan->sentMessageToAllMembers(modeChangeMessage_o2);
        }
      }
    } else {
      client->response(ERR_UNKNOWNMODE(client->getNickname(), args[2][0]));
      return false;
    }

    args[2].erase(0, 1);
  }
  return true;
}

bool Mode::execute(User *client, std::vector<std::string> args) {
  if (args.size() < 2) {
    client->response(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
    return false;
  }
  if (args[1][0] == '#') {
    args[1].erase(0, 1);
  } else {
    client->response(ERR_NOSUCHCHANNEL(client->getNickname(), args[1]));
    client->response("construction : 'MODE <#channel> <mode>' ");
    return false;
  }
  Channel *tmpChan = _srv->getChannelByName(args[1]);
  if (tmpChan == NULL) {
    client->response(ERR_NOSUCHCHANNEL(client->getNickname(), args[1]));
    return false;
  }
  if (!tmpChan->isInChannel(client)) {
    client->response(ERR_NOTONCHANNEL(client->getNickname(), args[1]));
    return false;
  }
  if (args.size() == 2) {
    client->response(RPL_CHANNELMODEIS(client->getNickname(),
                                       tmpChan->getChannelName(),
                                       tmpChan->getModeString()));
  }
  if (args.size() > 2) {
    if (execute_differents_modes(client, args, tmpChan) == false)
      return false;
  }
  return true;
}
