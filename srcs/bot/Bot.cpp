#include "../../headers/Bot.hpp"

#define host "localhost";

/***\ CONSTRUCTORS \***/

Bot::Bot(int port, std::string password)
    : _botPort(port), _botPassword(password) {
  this->_botMessages.push_back("Bienvenue a tout le monde sur le serveur !");
  this->_botMessages.push_back("Je suis un bot !");
  this->_nickname = "Sunbot";
  this->_username = "Sunbot";
  this->_realname = "elbot";
}

Bot::~Bot() {}

Bot::Bot(const Bot &src) { *this = src; }

Bot &Bot::operator=(const Bot &rhs) {
  if (this != &rhs) {
    this->_nickname = rhs._nickname;
    this->_username = rhs._username;
    this->_realname = rhs._realname;
    this->_botPort = rhs._botPort;
    this->_botPassword = rhs._botPassword;
    this->_botMessages = rhs._botMessages;
  }
  return *this;
}

/***\ METHODS \***/

bool Bot::connectToServer() {
  struct hostent *hostinfo;
  struct sockaddr_in server_addr;

  const char *hostname = host;
  if ((hostinfo = gethostbyname(hostname)) == NULL) {
    return (false);
  }

  this->_botSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->_botSocket == -1) {
    std::cerr << "Error: socket creation failed" << std::endl;
    return false;
  }
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(this->_botPort);
  server_addr.sin_addr = *(struct in_addr *)(hostinfo->h_addr);

  if (connect(this->_botSocket, (struct sockaddr *)(&server_addr),
              sizeof(struct sockaddr)) == -1) {
    std::cerr << "Error: connection failed" << std::endl;
    return false;
  }

  std::string password = "PASS " + this->_botPassword + "\r\n";
  std::string nickname = "NICK " + this->_nickname + "\r\n";
  std::string username = "USER " + this->_username + " 0 * " + this->_realname;
  std::string everything = password + nickname + username;

  this->response(everything);

  while (1) {
    char buffer[1024];
    memset(buffer, 0, 1024);

    ssize_t read = recv(this->_botSocket, buffer, 1024, 0);
    if (read < 0) {
      std::cout << RED "Error: Recv failed" NC << std::endl;
      exit(EXIT_FAILURE);
    }

    std::string strbuffer = buffer;

    std::cout << "J'AI RECU: " << strbuffer << std::endl;
  }
  return true;
}

void Bot::response(std::string message) {
  std::string response = message + "\r\n";
  send(this->_botSocket, response.c_str(), response.length(), 0);
}

int myAtoi3(char *str) {
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
    std::cout << YEL "Usage: ./bot <port> <password>" NC << std::endl;
    return -1;
  }
  Bot bot(myAtoi3(av[1]), av[2]);

  bot.connectToServer();
  return 0;
}