/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfaria-d <pfaria-d@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 13:24:28 by pfaria-d          #+#    #+#             */
/*   Updated: 2023/10/25 13:24:29 by pfaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_H
#define SOCKET_H

#include <string>

class Socket {
public:
  Socket();
  ~Socket();

  bool Create(int port);
  bool Bind();
  bool Listen(int maxConnections);
  bool Accept();
  bool Send(const std::string &message);
  int Receive(char *buffer, int bufferSize);

  void Close();

private:
  int serverSocket;
  int clientSocket;
  int port;
};

#endif
