/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 16:20:20 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/25 16:23:41 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string channelName) : _channelName(channelName) {
    // Constructeur
}

Channel::~Channel() {
    // Destructeur: Libération de ressources, si nécessaire
}

Channel::Channel(const Channel& cpy) : _channelName(cpy._channelName), _users(cpy._users) {
    // Constructeur de copie
}

Channel& Channel::operator=(const Channel& e) {
    if (this == &e) {
        return *this;  // retourne *this pour traiter l'affectation à soi-même
    }

    // Copie les attributs
    this->_channelName = e._channelName;
    this->_users = e._users;

    return *this;
}

int Channel::addUser(User &u)
{
    _users.push_back(u);
    return 0;
}