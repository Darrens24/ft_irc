/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfaria-d <pfaria-d@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 13:29:59 by pfaria-d          #+#    #+#             */
/*   Updated: 2023/10/25 13:30:01 by pfaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"
#include "../headers/utils.hpp"
#include <cstring>
#include <iostream>
#include <signal.h>
#include <string>
#include <unistd.h>

using namespace std;
bool server_up = true;

int main(int ac, char **av) {
  if (ac != 3) {
    cout << "Usage: ./ircserv <port> <password>" << endl;
    return 1;
  }

  signal(SIGINT, exit_server);
  Server server(myAtoi(av[1]), av[2]);
  server.start();

  return 0;
}
