#include "../../headers/Command.hpp"

Invite::Invite(Server *srv) : Command(srv) {}

Invite::~Invite() {}

// INVITE <nickname> <#channel>

bool Invite::execute(User *client, std::vector<std::string> args) {
  if (args.size() < 3) {
    client->response(ERR_NEEDMOREPARAMS(client->getNickname(), "INVITE"));
    return false;
  }
  if (args[2][0] == '#') {
    args[2].erase(0, 1);
  } else {
    client->response(ERR_NOSUCHCHANNEL(client->getNickname(), args[2]));
    client->response("construction : 'INVITE <nickname> <#channel>'");
    return false;
  }
  Channel *tmpChan = _srv->getChannelByName(args[2]);
  User *tmpUser = _srv->getUserByNickname(args[1]);
  if (tmpChan == NULL) {
    client->response(ERR_NOSUCHCHANNEL(client->getNickname(), args[2]));
    return false;
  }
  if (tmpUser == NULL) {
    client->response(ERR_NOSUCHNICK(client->getNickname(), args[1]));
    return false;
  }
  if (!tmpChan->isInChannel(client)) {
    client->response(ERR_NOTONCHANNEL(client->getNickname(), args[2]));
    return false;
  }
  if (client->isUserOperator(tmpChan) == false &&
      tmpChan->findMode('i') == true) {
    client->response(ERR_CHANOPRIVSNEEDED(client->getNickname(), args[2]));
    return false;
  }
  if (tmpChan->isInChannel(tmpUser)) {
    client->response(
        ERR_USERONCHANNEL(client->getNickname(), args[1], args[2]));
    return false;
  }
  tmpUser->setChannelInvited(tmpChan);
  client->response(RPL_INVITING(client->getNickname(), args[1], args[2]));

  std::string msg_invite =
      "You have been invited to join channel : " + tmpChan->getChannelName() +
      " by " + client->getNickname();
  tmpUser->response(msg_invite);

  return true;
}
