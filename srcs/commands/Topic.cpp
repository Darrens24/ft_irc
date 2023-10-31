/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 19:03:52 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/30 21:36:55 by feliciencat      ###   ########.fr       */
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
    std::cout << "wrong arguments" << std::endl;
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
            std::cout << "construction : 'TOPIC <#channel> :<topic>' " << std::endl;
            return false;
        }
        Channel *tmpChan = _srv->getChannelByName(args[1]);
        if (tmpChan == NULL)
        {
            std::cout << "Channel doesn't exist" << std::endl; // ERR_NOSUCHCHANNEL
            return false;
        }
        if (!tmpChan->isInChannel(client))
        {
            std::cout << "You are not in this channel" << std::endl; // ERR_NOTONCHANNEL
            return false;
        }
        if (client->isUserOperator(tmpChan) == false && tmpChan->findMode('t') == true)
        {
            std::cout << "You are not an operator" << std::endl;
            return false;
        }
        send (client->getFd(), "Topic of channel : ", 18, 0);
        send (client->getFd(), tmpChan->getChannelName().c_str(), tmpChan->getChannelName().length(), 0);
        send (client->getFd(), " is \'", 5, 0);
        send (client->getFd(), tmpChan->getTopic().c_str(), tmpChan->getTopic().length(), 0);
        send (client->getFd(), "\'\n", 2, 0);
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
        std::cout << "construction : 'TOPIC <#channel> :<topic>' " << std::endl;
        return false;
        }
    
        Channel *tmpChan = _srv->getChannelByName(args[1]);
    
        if (tmpChan == NULL)
        {
            std::cout << "Channel doesn't exist" << std::endl; // ERR_NOSUCHCHANNEL
            return false;
        }
        if (!tmpChan->isInChannel(client))
        {
            std::cout << "You are not in this channel" << std::endl; // ERR_NOTONCHANNEL
            return false;
        }
        if (client->isUserOperator(tmpChan) == false && tmpChan->findMode('t') == true)
        {
            std::cout << "You are not an operator" << std::endl;
            return false;
        }
        if (args[2] == "")
        {
            for (std::vector<User *>::iterator iter =
                 tmpChan->getUsersOfChannel().begin();
             iter != tmpChan->getUsersOfChannel().end(); iter++) {
            send ((*iter)->getFd(), "Topic of channel : ", 18, 0);
            send ((*iter)->getFd(), tmpChan->getChannelName().c_str(), tmpChan->getChannelName().length(), 0);
            send ((*iter)->getFd(), " as been cleared ", 19, 0);
            send ((*iter)->getFd(), "\n", 2, 0);
            tmpChan->setTopic("");}
        }
        else
        {
            tmpChan->setTopic(args[2]);
            for (std::vector<User *>::iterator iter =
                 tmpChan->getUsersOfChannel().begin();
             iter != tmpChan->getUsersOfChannel().end(); iter++) {
            send ((*iter)->getFd(), "Topic of channel : ", 18, 0);
            send ((*iter)->getFd(), tmpChan->getChannelName().c_str(), tmpChan->getChannelName().length(), 0);
            send ((*iter)->getFd(), " is now \'", 10, 0);
            send ((*iter)->getFd(), tmpChan->getTopic().c_str(), tmpChan->getTopic().length(), 0);
            send ((*iter)->getFd(), "\'\n", 2, 0);
            }
        }
    return true;   
    }
    

  
}