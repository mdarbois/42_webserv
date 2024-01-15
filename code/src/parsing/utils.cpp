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
  line.erase(line.find(";"), 1);
  std::istringstream  lineStream(line);
  int intNumber;
  lineStream >> intNumber;
  return intNumber;
}