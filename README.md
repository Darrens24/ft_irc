<p align="center">
    <img src="ft_ircm.png" alt="Badge Name">
</p>

## Project Overview

`ft_irc` is a custom implementation of an IRC (Internet Relay Chat) server, developed as a part of the 42 school curriculum. This project aims to deepen understanding of network programming, socket handling, and the implementation of the RFC 1459 (IRC) protocol. The server is designed to handle multiple clients simultaneously, providing a robust and efficient communication platform.

## Features and Bonuses

- **RFC 1459 Compliance**: Our implementation closely follows the RFC 1459 specifications, ensuring compatibility with standard IRC clients.
- **Multi-client Handling**: Utilizes non-blocking sockets and `select` system call to manage multiple client connections efficiently.
- **Robust Command Parsing**: Implements a comprehensive parser to handle a wide array of IRC commands and client requests.
- **Channel Management**: Supports creation, joining, leaving, and managing of channels, including private channels as a bonus feature.
- **Private Messaging**: Implements private messaging between users, including bonus features such as message notifications.
- **File Transfer**: Allows users to send and receive files via Irssi
- **User Authentication**: Includes optional user authentication and registration mechanisms as part of the bonus objectives.
- **Logging and Monitoring**: Enhanced logging capabilities for server monitoring and debugging, going beyond basic requirements.

## Technologies Used

- **C/C++ Programming**: Core implementation in C, with some C++ elements for advanced features.
- **Network Programming**: Extensive use of socket programming and Unix network APIs.
- **Multithreading/Multiprocessing**: Utilization of multithreading/multiprocessing concepts to handle concurrent client connections.

## Challenges Faced

- **Concurrency Handling**: Managing multiple client connections simultaneously while ensuring data consistency was a significant challenge.
- **RFC Compliance**: Understanding and accurately implementing the IRC protocol as per RFC 1459 required meticulous attention to detail.
- **Efficient Parsing**: Developing an efficient command parser to handle various IRC commands was a complex task.

## Installation and Usage

The reference client that we used is Irssi, and it was also tested on Netcat.

Instructions for setting up and running the `ft_irc` server:
```bash
git clone git@github.com:Darrens24/ft_irc.git 
cd ft_irc
make
make bot
./irc_server [port] [password]
./bot [port] [password]
```
