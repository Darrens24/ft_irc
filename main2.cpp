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

#include "Server.hpp"
#include "Socket.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

int myAtoi2(char *str) {
  int res = 0;
  int sign = 1;
  int i = 0;

  if (str[0] == '-') {
    sign = -1;
    i++;
  }
  for (; str[i] != '\0'; ++i)
    res = res * 10 + str[i] - '0';
  return sign * res;
}

int main(int ac, char **av) {
  if (ac != 3) {
    cout << "Usage: ./ircserv <port> <password>" << endl;
    return 1;
  }

  Server server(myAtoi2(av[1]), av[2]);
  server.start();

  return 0;
}
