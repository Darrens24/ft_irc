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

Channel::Channel(std::string channelName) : _channelName(channelName) {
  // Constructeur
  key = "";
}

Channel::~Channel() {
  // Destructeur: Libération de ressources, si nécessaire
}

Channel::Channel(const Channel &cpy)
    : _channelName(cpy._channelName), _users(cpy._users) {
  // Constructeur de copie
}

Channel &Channel::operator=(const Channel &e) {
  if (this == &e) {
    return *this; // retourne *this pour traiter l'affectation à soi-même
  }

  // Copie les attributs
  this->_channelName = e._channelName;
  this->_users = e._users;

  return *this;
}

int Channel::addUser(User *u) {
  _users.push_back(u);
  return 0;
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

void Channel::sentMessageToAllMembers(std::string message)
{
  for (std::vector<User *>::iterator it = _users.begin(); it != _users.end();
       it++) {
    (*it)->response(message);
  }
}


void Channel::setOwner(User *u) { _owner = u; }

User *Channel::getOwner() { return _owner; }

std::string Channel::getChannelName() { return _channelName; }

std::vector<User *> &Channel::getUsersOfChannel() { return this->_users; }

std::string Channel::getKey() { return this->key; }

void Channel::setKey(std::string key) { this->key = key; }

void Channel::responseALL(std::string response) {
  for (std::vector<User *>::iterator it = _users.begin(); it != _users.end();
       it++) {
    (*it)->response(response);
  }
}

void Channel::responseALLnotMe(std::string response, std::string nick) {
  for (std::vector<User *>::iterator it = _users.begin(); it != _users.end();
       it++) {
    if (nick != (*it)->getNickname())
      (*it)->response(response);
  }
}
