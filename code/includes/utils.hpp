#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <unistd.h>
#include <string>
#include <sstream>

void trimComments(std::string &content);
void trimSpaces(std::string &content);
std::string extractString(std::string &line, size_t pos, size_t length);
unsigned int extractInt(std::string &line, size_t pos, size_t length);
#endif