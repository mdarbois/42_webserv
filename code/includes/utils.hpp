#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <unistd.h>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <algorithm>
#include <fstream>


void trimComments(std::string &content);
void trimSpaces(std::string &content);
std::string extractString(std::string &line, size_t pos, size_t length);
unsigned int extractInt(std::string &line, size_t pos, size_t length);
bool checkSemiColon(const std::string& line);
std::string intToString(const int &number);

#endif