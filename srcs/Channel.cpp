/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 16:20:20 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/27 12:25:44 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Channel.hpp"

Channel::Channel(std::string channelName) : _channelName(channelName) {
  // Constructeur
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
