/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 13:00:52 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/25 13:54:34 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"

//create constructor
User::User()
{
    _addrClientSize = sizeof(_addrClient);
    _userfd[0].fd = _socketClient;
    _userfd[0].events = POLLIN;
    _ret = poll(_userfd, 1, 1000);
}

int User::init()
{
    _username = "";
    while (_username.empty())
    {
        char buffer[1024] = {0};

        if (_ret > 0)
        {
            ssize_t bytes_received = recv(_socketClient, buffer, 1024, 0);
            if (bytes_received < 0) {
                std::cout << RED "Recv failed" NC << std::endl;
                return 1; // Ou vous pouvez gérer l'erreur d'une autre manière
            }

            // Supprimer les caractères de nouvelle ligne ou de retour chariot si nécessaire
            for (ssize_t i = 0; i < bytes_received; ++i)
            {
                if (buffer[i] == '\n' || buffer[i] == '\r')
                {
                    buffer[i] = '\0';
                    break;
                }
            }

            _username = std::string(buffer);
            if (_username.empty())
            {
                std::cout << "Username cannot be empty. Please enter again." << std::endl;
                send(_socketClient, "Username cannot be empty. Please enter again.\n", 44, 0);
            }
            else
            {
                std::cout << "Username received: " << _username << std::endl;
                break; // sortir de la boucle
            }
        }
}
    return 0;
}
    
User::~User()
{
}

User::User(const User& cpy)
{   
    *this = cpy;   
}

User &User::operator=(const User& e)
{
    if (this == &e)
        return (*this);
    this->_addrClient = e._addrClient;
    this->_addrClientSize = e._addrClientSize;
    this->_socketClient = e._socketClient;
    this->_userfd[0].fd = e._userfd[0].fd;
    this->_userfd[0].events = e._userfd[0].events;
    this->_username = e._username;   
    return(*this);   
}
