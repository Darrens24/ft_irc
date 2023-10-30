#include "../../headers/Command.hpp"

Part::Part(Server *srv) : Command(srv) {}

Part::~Part() {}

bool Part::execute(User *client, std::vector<std::string> args) {

  if (args.size() < 2) {
    client->response(ERR_NEEDMOREPARAMS(client->getNickname(), "PART"));
  }

  std::cout << "we enter PART" << std::endl;

  std::string channelName = args[1];
  channelName.erase(0, 1);
  std::cout << "channelName: " << channelName << std::endl;

  for (std::map<std::string, Channel *>::iterator it =
           this->_srv->getChannels().begin();
       it != this->_srv->getChannels().end(); ++it) {
    if (it->first == channelName) {
      Channel *channel = it->second;
      for (std::vector<User *>::iterator it2 =
               channel->getUsersOfChannel().begin();
           it2 != channel->getUsersOfChannel().end(); ++it2) {
        if ((*it2)->getNickname() == client->getNickname()) {
          channel->removeUserFromChannel(client);
          std::string msg = "[" + channelName + "] You left the channel!";
          client->response(msg);
          return true;
        }
      }
      client->response(ERR_NOTONCHANNEL(client->getNickname(), channelName));
      return false;
    }
  }
  client->response(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
  return false;
}
