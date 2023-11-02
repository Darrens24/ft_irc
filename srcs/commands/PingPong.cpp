#include "../../headers/Command.hpp"

Ping::Ping(Server *srv) : Command(srv) {}

Ping::~Ping() {}

bool Ping::execute(User *client, std::vector<std::string> args) {
  if (args.size() < 1) {
    client->response(ERR_NEEDMOREPARAMS(client->getHostname(), "PING"));
    return false;
  }

  client->response("PONG " + args[1]);
  return true;
}
