#pragma once

#include "../headers/Server.hpp"
#include <signal.h>
#include <sstream>
#include <string>
#include <vector>

int myAtoi(char *str);
std::string myStoi(int i);
int myStrAtoi(std::string str);
void exit_server(int signal);
std::vector<std::string> mySplit(std::string str, std::string sep);
