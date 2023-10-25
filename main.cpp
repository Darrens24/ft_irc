/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfaria-d <pfaria-d@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 18:50:58 by pfaria-d          #+#    #+#             */
/*   Updated: 2023/10/24 18:50:59 by pfaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

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

using namespace std;
int main(int ac, char **av) {
  if (ac != 3) {
    cout << YEL "Usage: ./ircserv <port> <password>" NC << endl;
    return 1;
  }

  int socketServer = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addrServer;
  addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
  addrServer.sin_family = AF_INET;
  addrServer.sin_port = htons(myAtoi(av[1]));

  // Bind
  if (bind(socketServer, (struct sockaddr *)&addrServer, sizeof(addrServer)) <
      0) {
    cout << RED "Bind failed" NC << endl;
    return 1;
  }

  // Listen
  if (listen(socketServer, 7) < 0) {
    cout << RED "Listen failed" NC << endl;
    return 1;
  }

  // Accept
  int socketClient;
  struct sockaddr_in addrClient;
  socklen_t addrClientSize = sizeof(addrClient);
  if ((socketClient = accept(socketServer, (struct sockaddr *)&addrClient,
                             &addrClientSize)) < 0) {
    cout << RED "Accept failed" NC << endl;
    return 1;
  }

  // Send
  send(socketClient, "hello\n", 6, 0);
  cout << "Hello message sent" << endl;

  while (1) {
    // Read with poll
    char buffer[1024] = {0};
    pollfd fds[1];
    fds[0].fd = socketClient;
    fds[0].events = POLLIN;
    int ret = poll(fds, 1, 1000);
    if (ret > 0) {
      if (recv(socketClient, buffer, 1024, 0) < 0) {
        cout << RED "Recv failed" NC << endl;
        return 1;
      }
      cout << buffer;
      if (!strcmp(buffer, "exit\n")) {
        cout << "Exiting" << endl;
        break;
      }
    }
  }

  // Close
  close(socketServer);
  close(socketClient);

  return 0;
}
