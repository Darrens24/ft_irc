/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 19:03:52 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/31 15:24:00 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Topic::Topic(Server *srv) : Command(srv) {}

Topic::~Topic() {}

// TOPIC <#channel> :<topic>

/*If the topic of a channel is changed or cleared, every client in that channel
(including the author of the topic change) will receive a TOPIC command with the new topic as
argument (or an empty argument if the topic was cleared) alerting them to how the topic has changed.
*/

bool Topic::execute(User *client, std::vector<std::string> args) {
  
  if (args.size() < 2 || args.size() > 4) {
    client->response(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
    return false;
  }
  //if only one argument, return the topic of the channel
    if (args.size() == 2)
    {
        if (args[1][0] == '#')
        {
            args[1].erase(0, 1);
        }
        else
        {
            client->response(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
            client->response("construction : 'TOPIC <#channel> :<topic>' ");
            return false;
        }
        Channel *tmpChan = _srv->getChannelByName(args[1]);
        if (tmpChan == NULL)
        {
            client->response(ERR_NOSUCHCHANNEL(client->getNickname(), args[1]));
            return false;
        }
        if (!tmpChan->isInChannel(client))
        {
            client->response(ERR_NOTONCHANNEL(client->getNickname(), args[1]));
            return false;
        }
        if (client->isUserOperator(tmpChan) == false && tmpChan->findMode('t') == true)
        {
            client->response(ERR_CHANOPRIVSNEEDED(client->getNickname(), args[1]));
            return false;
        }
        std::string msg_set = "Topic of channel : " + tmpChan->getChannelName() + " is \'" + tmpChan->getTopic() + "\'";
        client->response(msg_set);
        return true;
    }
    else
    {
        if (args[1][0] == '#' && args[2][0] == ':')
        {
            args[1].erase(0, 1);
            args[2].erase(0, 1);
        }
        else 
        {
        client->response(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
        client->response("construction : 'TOPIC <#channel> :<topic>' ");
        return false;
        }
    
        Channel *tmpChan = _srv->getChannelByName(args[1]);
    
        if (tmpChan == NULL)
        {
            client->response(ERR_NOSUCHCHANNEL(client->getNickname(), args[1]));
            return false;
        }
        if (!tmpChan->isInChannel(client))
        {
            client->response(ERR_NOTONCHANNEL(client->getNickname(), args[1]));
            return false;
        }
        if (client->isUserOperator(tmpChan) == false && tmpChan->findMode('t') == true)
        {
            client->response(ERR_CHANOPRIVSNEEDED(client->getNickname(), args[1]));
            return false;
        }
        if (args[2] == "")
        {
            std::string message_cleared = "Topic of channel  \'" + tmpChan->getChannelName() + "\' as been cleared\n";
            tmpChan->sentMessageToAllMembers(message_cleared);
            tmpChan->setTopic("");
            return true;
        }
        else
        {
            tmpChan->setTopic(args[2]);
            std::string message_new_topic  = "Topic of channel \'" + tmpChan->getChannelName()
                    + "\' is now : " + tmpChan->getTopic() + "\n";
            tmpChan->sentMessageToAllMembers(message_new_topic);
            return true;
        }
    return true;   
    }
    

  
}