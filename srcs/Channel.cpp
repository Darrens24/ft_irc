/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 16:20:20 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/31 10:17:15 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Channel.hpp"

/***\ CONSTRUCTORS \***/

Channel::Channel(std::string channelName) : _channelName(channelName) {
  key = "";
}

Channel::~Channel() {}

Channel::Channel(const Channel &cpy) { *this = cpy; }

Channel &Channel::operator=(const Channel &e) {
  if (this == &e) {
    return *this;
  }

  this->_channelName = e._channelName;
  this->_users = e._users;
  this->_owner = e._owner;
  this->key = e.key;
  this->_topic = e._topic;
  this->_mode = e._mode;

  return *this;
}

/***\ METHODS \***/

int Channel::addUser(User *u) {
  _users.push_back(u);
  return 0;
}

void Channel::removeUserFromChannel(User *u) {
  for (std::vector<User *>::iterator it = _users.begin(); it != _users.end();
       it++) {
    if (*it == u) {
      _users.erase(it);
      return;
    }
  }
}

int Channel::isInChannel(User *u) {
  for (std::vector<User *>::iterator it = _users.begin(); it != _users.end();
       it++) {
    if (*it == u)
      return 1;
  }
  return 0;
}

bool Channel::removeMode(char mode) {
  for (std::vector<char>::iterator it = _mode.begin(); it != _mode.end();
       it++) {
    if (*it == mode) {
      _mode.erase(it);
      return true;
    }
  }
  return false;
}

std::string Channel::getModeString() {
  std::string mode = "";
  for (std::vector<char>::iterator it = _mode.begin(); it != _mode.end();
       it++) {
    mode += *it;
  }
  return mode;
}

bool Channel::addMode(char mode) {
  for (std::vector<char>::iterator it = _mode.begin(); it != _mode.end();
       it++) {
    if (*it == mode) {
      return false;
    }
  }
  _mode.push_back(mode);
  return true;
}

bool Channel::findMode(char mode) {
  for (std::vector<char>::iterator it = _mode.begin(); it != _mode.end();
       it++) {
    if (*it == mode) {
      return true;
    }
  }
  return false;
}

void Channel::responseALL(std::string response) {
  for (std::vector<User *>::iterator it = _users.begin(); it != _users.end();
       it++) {
    (*it)->response(response);
  }
}

void Channel::sentMessageToAllMembers(std::string message) {
  for (std::vector<User *>::iterator it = _users.begin(); it != _users.end();
       it++) {
    (*it)->response(message);
  }
}

void Channel::responseALLnotMe(std::string response, std::string nick) {
  for (std::vector<User *>::iterator it = _users.begin(); it != _users.end();
       it++) {
    if (nick != (*it)->getNickname())
      (*it)->response(response);
  }
}

/***\ SETTERS \***/

void Channel::setOwner(User *u) { _owner = u; }

void Channel::setKey(std::string key) { this->key = key; }
