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

#include "Socket.hpp"
#include <iostream>
#include <string>

using namespace std;

int myAtoi(char *str) {
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

  int port = myAtoi(av[1]);

  string pass = string(av[2]);
  Socket socket(pass);
  if (!socket.Create(port) || !socket.Bind() || !socket.Listen(7)) {
    return 1;
  }

  if (!socket.Accept()) {
    return 1;
  }

  if (!socket.Send("hello\n")) {
    return 1;
  }

  cout << "Hello message sent" << endl;

  while (true) {
    char buffer[1024] = {0};
    int receivedBytes = socket.Receive(buffer, 1024);
    if (receivedBytes < 0) {
      return 1;
    }

    if (receivedBytes > 0) {
      cout << buffer;
      if (string(buffer) == "exit\n") {
        cout << "Exiting" << endl;
        break;
      }
    }
  }

  return 0;
}
