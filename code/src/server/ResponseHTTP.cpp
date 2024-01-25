/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:41:03 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/25 10:57:14 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ResponseHTTP.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
ResponseHTTP::ResponseHTTP() {}

ResponseHTTP::ResponseHTTP(ParserHTTP request, ServerConfig config)
{
	//Config needed, to check if the Method is allowed for the location
	_config = config;
	_request = request;
	//Very basic. A lot of cheecks have to be performed
	if (!_isValidRequest())
		_createErrorResponse();
	else if (_request.isCGI())
		std::cout << "Handle CGI later :)" << std::endl;
	else if (request.getMethod() == GET)
		_GET();
	else if (request.getMethod() == POST)
		_POST();
	else if (request.getMethod() == DELETE)
		_DELETE();
}
/* ResponseHTTP::ResponseHTTP()
{
} */

/* ResponseHTTP::ResponseHTTP( const ResponseHTTP & src )
{
} */


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ResponseHTTP::~ResponseHTTP()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ResponseHTTP &				ResponseHTTP::operator=( ResponseHTTP const & rhs )
{
	if ( this != &rhs )
	{
		_config = rhs._config;
		_responseLine = rhs._responseLine;
		_body = rhs._body;
		_header = rhs._header;
		_bodyLength = rhs._bodyLength;
	}
	return *this;
}

/* std::ostream &			operator<<( std::ostream & o, ResponseHTTP const & i )
{
	//o << "Value = " << i.getValue();
	return o;
} */


/*
** --------------------------------- METHODS ----------------------------------
*/

void	ResponseHTTP::_createErrorResponse()
{
	//Later check which error html should be chosen
	std::string	fullPath = "/html/404.html"; //TESTING!!! Change later
	_request.overrideReqPathtoErrorPath(fullPath);
	_readFile();
	_header["Content-Type"] = std::string("text/html");
	setResponseLine(HTTP_404, "File Not Found");
}

bool	ResponseHTTP::_isValidRequest()
{
	std::string	fullPath = "." + _request.getPath(); //TESTING!!! Change later
	//check if the requested resource exists
	if (access(fullPath.c_str(), F_OK) != 0)
		return (false);
	return (true);
}

bool	ResponseHTTP::_readFile()
{
	std::string	fullPath = "." + _request.getPath(); //TESTING!!! Change later
	std::ifstream	htmlFile(fullPath.c_str());

	if (!htmlFile.is_open())
		return (false);

	// Determine the size of the file
	htmlFile.seekg(0, std::ios::end);  // Move to the end of the file
	int fileSize = static_cast<int>(htmlFile.tellg());  // Get the current position (which is the file size)
	htmlFile.seekg(0, std::ios::beg);  // Move back to the beginning of the file

	char* buffer = new char[fileSize];
	if (!htmlFile.read(buffer, fileSize))
	{
		htmlFile.close();
		return (false);
	}
	_header["Content-Length"] = intToString(fileSize);
	_bodyLength = fileSize;
	_body = std::string(buffer, fileSize);
	delete []buffer;
	htmlFile.close();
	return (true);
}

void	ResponseHTTP::_GET()
{
	/* if (_request.isCGI())
	{
		//Do CGI Stuff
	} */
	//Check Autoindex
	
	//For now just test the normal Get action, to read the requested file
	if (!_readFile())
		throw std::runtime_error("Could not open HTML file");
	else
	{
		 //This needs to be set later depending on the requested resource
		_header["Content-Type"] = std::string("text/html");
		setResponseLine(HTTP_200, "OK");
	}
}

void	ResponseHTTP::_POST()
{
	//check if the path is a folder, ends with a /
	if (_request.getPath().empty() || _request.getPath()[_request.getPath().length() - 1] != '/')
		setResponseLine(HTTP_400, "Bad Request");

	//check if the folder is accessible and the rights to post a file 
	std::string	fullPath = "." + _request.getPath(); //TESTING!!! Change later
	if (access(fullPath.c_str(), F_OK | W_OK) != 0)
		setResponseLine(HTTP_404, "Not Found");

	//check if the location supports POST
	if (!containsValue<std::string>(_config.getLocations()[_request.getPath()].getMethods(), "POST"))
		setResponseLine(HTTP_403, "Forbidden");

	//create a new file
	std::ofstream	newFile("./testUpload");
}

void	ResponseHTTP::_DELETE()
{
	
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
std::string	ResponseHTTP::getFullResponseString() const
{
	std::string responseStr = ALLOWED_HTTP_PROTOCOL;
	responseStr.append(" ");
	responseStr.append(intToString(_responseLine.httpStatusCode));
	responseStr.append(" ");
	responseStr.append(_responseLine.reasonPhrase);
	responseStr.append("\r\n");
	for (std::map<std::string, std::string>::const_iterator it = _header.begin(); it != _header.end(); it++)
	{
		responseStr.append(it->first);
		responseStr.append(": ");
		responseStr.append(it->second);
		responseStr.append("\r\n");
	}
	responseStr.append("\r\n");
	responseStr.append(_body);
	return (responseStr);
}

std::map<std::string, std::string>	ResponseHTTP::getResponseHeaderMap() const
{
	return (_header);
}

int	ResponseHTTP::getBodyLength() const
{
	return (_bodyLength);
}

int	ResponseHTTP::getResponseLength() const
{
	return (getFullResponseString().length());
}

void	ResponseHTTP::setBody(std::string body)
{
	_body = body;
}

void	ResponseHTTP::setResponseLine(HttpStatus status, std::string reasonPhrase)
{
	_responseLine.allowedHttpProtocol = ALLOWED_HTTP_PROTOCOL;
	_responseLine.httpStatusCode = status;
	_responseLine.reasonPhrase = reasonPhrase;
}

void	ResponseHTTP::setHeader(std::string key, std::string value)
{
	_header[key] = value;
}

/* ************************************************************************** */