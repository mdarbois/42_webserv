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
#include <map>


void trimComments(std::string &content);
void trimSpaces(std::string &content);
std::string extractString(std::string &line, size_t pos, size_t length);
unsigned int extractInt(std::string &line, size_t pos, size_t length);
bool checkSemiColon(const std::string& line);
std::string intToString(const int &number);
std::vector<std::string> mapToArray(const std::map<std::string, std::string>& myMap);
char** vectorToCharArray(const std::vector<std::string>& strVector);
void deleteArray(char** charArray);
std::string pairToString(const std::string& key, const std::string& value);
template <typename T>
bool	containsValue(const std::vector<T>& vec, const T& value) {
	return std::find(vec.begin(), vec.end(), value) != vec.end();

}
std::vector<std::string> splitString(const std::string& s, char delimiter);
void prependCharacter(std::vector<std::string>& vec, char ch);

#endif