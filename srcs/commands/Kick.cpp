#include "../../headers/Command.hpp"

// KICK #channelname username :reason

Kick::Kick(Server *srv) : Command(srv) {}

Kick::~Kick() {}

std::vector<std::string> myOwnSplit_kick(std::string str, std::string sep) {
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

bool Kick::execute(User *client, std::vector<std::string> args) {
  if (args.size() < 3 || args.size() > 4) {
    client->response(ERR_NEEDMOREPARAMS(client->getNickname(), "KICK"));
    return false;
  }
  if (args[1][0] == '#') {
    args[1].erase(0, 1);
  } else {
    client->response(ERR_NOSUCHCHANNEL(client->getNickname(), args[1]));
    client->response("construction : 'KICK #channelname username [:reason]'");
    return false;
  }
  bool reason = false;
  if (args.size() == 4 && args[3].length() > 0) {
    if (args[3][0] == ':') {
      args[3].erase(0, 1);
      reason = true;
    } else {
      client->response(ERR_NEEDMOREPARAMS(client->getNickname(), "KICK"));
      client->response("construction : 'KICK #channelname username [:reason]'");
      return false;
    }
  }

  bool found_user = false;

  Channel *tmpChan = _srv->getChannelByName(args[1]);
  if (tmpChan == NULL) {
    client->response(ERR_NOSUCHCHANNEL(client->getNickname(), args[1]));
    return false;
  }

  std::vector<std::string> users_split = myOwnSplit_kick(args[2], ",");

  for (std::vector<std::string>::iterator it = users_split.begin();
       it != users_split.end(); it++) {

    User *tmpUser = _srv->getUserByNickname(*it);
    if (tmpUser == NULL) {
      client->response(ERR_NOSUCHNICK(client->getNickname(), *it));
      return false;
    }
    if (!tmpChan->isInChannel(client)) {
      client->response(ERR_NOTONCHANNEL(client->getNickname(), args[1]));
      return false;
    }
    if (client->isUserOperator(tmpChan) == false) {
      client->response(ERR_CHANOPRIVSNEEDED(client->getNickname(), args[1]));
      return false;
    }
    if (tmpUser->getNickname() == client->getNickname()) {
      client->response("You can't kick yourself");
      return false;
    }
    for (std::vector<User *>::iterator it =
             tmpChan->getUsersOfChannel().begin();
         it != tmpChan->getUsersOfChannel().end(); it++) {
      if ((*it)->getNickname() == tmpUser->getNickname()) {
        std::string msg = "";
        if (reason == false)
          // msg = "You have been kicked by " + client->getNickname();
          msg = tmpUser->getNickname() + " has been kicked by " +
                client->getNickname();

        else {
          // msg = "You have been kicked by " + client->getNickname() + " for "
          // +
          //       args[3];
          msg = tmpUser->getNickname() + " has been kicked by " +
                client->getNickname() + " for " + args[3];
        }
        // (*it)->response(msg);
        Privmsg privmsg(_srv);
        std::string chann = "#" + tmpChan->getChannelName();
        std::vector<std::string> array_msg;
        array_msg.push_back("PRIVMSG");
        array_msg.push_back(chann);
        array_msg.push_back(msg);
        privmsg.execute_msg(client, chann, array_msg);
        tmpChan->getUsersOfChannel().erase(it);
        // tmpChan->sentMessageToAllMembers(tmpUser->getNickname() +
        //                                  " has been kicked by " +
        //                                  client->getNickname());
        found_user = true;
        break;
      }
    }
    if (found_user == false) {
      client->response("user not in channel");
      return false;
    }
  }
  return true;
}
