/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:47:52 by feliciencat       #+#    #+#             */
/*   Updated: 2023/10/27 16:05:31 by feliciencat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Invite::Invite(Server *srv) : Command(srv) {}

Invite::~Invite()
{}

void Invite::execute(User *client, std::vector<std::string> args)
{
    if(args.size() < 3)
    {
    std::cout << "Not enough arguments" << std::endl;
    return;
    }
    if (args[1][0] == '#') { 
        args[1].erase(0, 1);
    }
    else {
        std::cout << "construction : 'INVITE #channelname username' " << std::endl;
        return;
    }

}
