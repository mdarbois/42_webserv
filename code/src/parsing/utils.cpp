#include "../../includes/utils.hpp"

void trimComments(std::string &content)
{
  	size_t pos_start;

	pos_start = content.find('#');
	while (pos_start != std::string::npos)
	{
		size_t pos_end;
		pos_end = content.find('\n', pos_start);
		content.erase(pos_start, pos_end - pos_start);
		pos_start = content.find('#');
	}
}

void trimSpaces(std::string &content)
{
	size_t	i = 0;

	while (content[i] && isspace(content[i]))
		i++;
	content = content.substr(i);
	if (content.empty())
        return;
	i = content.length() - 1;
	while (i > 0 && isspace(content[i]))
		i--;
	content = content.substr(0, i + 1);
}

std::string extractString(std::string &line, size_t pos, size_t length)
{
	trimSpaces(line);
  	if (pos != std::string::npos)
		line.erase(pos, length);
	if (line.find(";") != std::string::npos)
  		line.erase(line.find(";"), 1);
 	trimSpaces(line);
	return (line);
}

unsigned int extractInt(std::string &line, size_t pos, size_t length)
{
	if (pos != std::string::npos)
		line.erase(pos, length);
	trimSpaces(line);
	if (line.find(";") != std::string::npos)
		line.erase(line.find(";"), 1);
	std::istringstream  lineStream(line);
	int intNumber;
	lineStream >> intNumber;
	return intNumber;
}

bool checkSemiColon(const std::string& line) {
	if (!line.empty() && line.find("server {") == std::string::npos && line.find("location") == std::string::npos && line.find("}") == std::string::npos)
	{
		if (line[line.length() - 1] != ';')
			return false;
		else
			return true;
	}
	else
		return true;
}

std::string intToString(const int &number)
{

	std::stringstream ss;
	ss << number;
	return (ss.str());
}

std::vector<std::string> mapToArray(const std::map<std::string, std::string>& myMap)
{
    std::vector<std::string> result;

    for (std::map<std::string, std::string>::const_iterator it = myMap.begin(); it != myMap.end(); ++it) {
        result.push_back(it->first + ": " + it->second);
    }
    return result;
}

char** vectorToCharArray(const std::vector<std::string>& strVector) {
    // Create a dynamic array of char pointers
    char** charArray = new char*[strVector.size() + 1];  // +1 for the terminating NULL pointer

    // Copy each string from the vector to the dynamic array
    for (size_t i = 0; i < strVector.size(); ++i) {
        // Allocate memory for each string and copy it
        charArray[i] = new char[strVector[i].length() + 1];
        std::strcpy(charArray[i], strVector[i].c_str());
    }

    // Add a terminating NULL pointer at the end
    charArray[strVector.size()] = NULL;

    return charArray;
}

void deleteArray(char** charArray)
{
	for (size_t i = 0; charArray[i] != NULL; ++i) {
			delete[] charArray[i];
		}
		delete[] charArray;

} 

std::string pairToString(const std::string& key, const std::string& value) {
    std::stringstream ss;
    ss << key << "=" << value;
    return ss.str();
}

std::vector<std::string> splitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string::size_type start = 0;
    std::string::size_type end = s.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(s.substr(start, end - start + 1));
        start = end + 1;
        end = s.find(delimiter, start);
    }

    // Push the last token (or the only token if no delimiter is found)
    tokens.push_back(s.substr(start));

    return tokens;
}

// Function to prepend a character to each string in a vector
void prependCharacter(std::vector<std::string>& vec, char ch) {
    for (size_t i = 0; i < vec.size(); ++i) {
        vec[i] = ch + vec[i]; // Prepend the character to each string
    }
}

bool endsWithSlash(const std::string& str) {
    if (!str.empty()) {
        // Check if the last character is '/'
        return str[str.length() - 1] == '/';
    }
    return false; // Return false for an empty string
}