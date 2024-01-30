/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:41:03 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/30 14:21:30 by aehrlich         ###   ########.fr       */
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
	if (_request.isCGI())
	{
    //CGI cgi(_request);
		_cgi = CGI(_request);
		setResponseLine(HTTP_200, "OK");
		_body = _cgi.getBody();
	}
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

void	ResponseHTTP::_createErrorResponse(std::string errPagePath, HttpStatus status)
{
	_request.overidePath(errPagePath);
	_readFile();
	setResponseLine(status, "Retreive later from a lookup");
}

bool	ResponseHTTP::_readFile()
{
	std::ifstream	htmlFile(getFullRequestedPath().c_str());
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

PathType	getPathType(std::string path)
{
	struct stat pathStat;
	if (stat(path.c_str(), &pathStat) != 0)
		return (PT_ERROR);
	if (S_ISREG(pathStat.st_mode))
		return (PT_FILE);
	else if (S_ISDIR(pathStat.st_mode))
		return (PT_DIR);
	return (PT_ERROR);
}


void	ResponseHTTP::_GET()
{
	
	
	// Check redirection
	std::string pathNoRoot = _request.getPath().erase(0,5);
	pathNoRoot.erase(pathNoRoot.length() - 1);
	//std::cout << "PATH=" << pathNoRoot << std::endl;
	std::map<std::string, LocationConfig> locations(_config.getLocations());
	for (std::map<std::string, LocationConfig>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
		//std::cout << "\033[92m---------- Location: " << it->first << " -----------\033[0m\n";
        //std::cout << std::endl;
		if(it->first == pathNoRoot && !(it->second.getRedirection().empty()))
		{
			std::string statusCode = (it->second).getRedirection().substr(0,3);
			setResponseLine(static_cast<HttpStatus>(std::atoi(statusCode.c_str())), "Moved Permanently");
			std::string link = (it->second).getRedirection().erase(0,3);
			trimSpaces(link);
			std::string htmlContent =
				"<html>"
				"<head>"
				"<title>Redirecting...</title>"
				"</head>"
				"<body>"
				"<p>This page has moved. Please follow <a href=\"" + link + "\">this link</a>.</p>"
				"</body>"
				"</html>";
			setBody(htmlContent);
			return ;
		}
	}
	//Check if the requested Resource is existing
	if (access(getFullRequestedPath().c_str(), F_OK) != 0)
		return (_createErrorResponse("/html/404.html", HTTP_404));
	//TBD: HOW TO CHECK THE ALLOWED METHODS HERE?
	//Check if file or directory is requested
	PathType requestedResource = getPathType(getFullRequestedPath());
	if (requestedResource == PT_ERROR)
		throw std::runtime_error("GET: Could not determine file or dir type");
	else if (requestedResource == PT_FILE)
	{
		//Reads the requested file to the body
		if (!_readFile())
			throw std::runtime_error("GET: Could not open file");
		else
			setResponseLine(HTTP_200, "OK");
	}
	else if (requestedResource == PT_DIR)
	{
		if (getFullRequestedPath()[getFullRequestedPath().length() - 1] != '/')
		{
			std::cout << "What should we do here? :)" << std::endl;
		}
		else
		{
			if (!_config.getLocations()[_request.getPath()].getIndex().empty())
			{
				//override the path/
				//std::cout << "My location:" << std::endl;
				//std::cout << _config.getLocations()[_request.getPath()] << std::endl;
				if (!_readFile())
					throw std::runtime_error("GET: Could not open file");
				else
					setResponseLine(HTTP_200, "OK");
			}
			else
			{
				if (!_config.getLocations()[_request.getPath()].getAutoindex())
				{
					//No index and no auto index
					//return the error page of this location or the default error page
					//Error 403
					_request.overidePath("/html/403.html"); //make it dynamically later
					if (!_readFile())
						throw std::runtime_error("GET: Could not open file");
					else
						setResponseLine(HTTP_403, "Forbidden");
				}
				else
				{
					//return the auto index html with all the subfolders and
				}
			}
			
		}
	}
}

void	ResponseHTTP::_POST()
{
	//check if the path is a folder, ends with a /
	if (_request.getPath().empty() || _request.getPath()[_request.getPath().length() - 1] != '/')
		return setResponseLine(HTTP_400, "Bad Request");

	//check if the folder is accessible and the rights to post a file 
	if (access(getFullRequestedPath().c_str(), F_OK | W_OK) != 0)
		return setResponseLine(HTTP_404, "Not Found");

	//check if the location supports POST
	if (!containsValue<std::string>(_config.getLocations()[_request.getPath()].getMethods(), "POST"))
		return setResponseLine(HTTP_403, "Forbidden");

	//create a new file
	std::string	uploadFilePath = std::string("./uploads/") + _request.getUploadFilename();
	std::ofstream	newFile(uploadFilePath.c_str());
	if (!newFile.is_open())
		throw std::runtime_error("Could not create posted file");
	setResponseLine(HTTP_200, "OK");
}

void	ResponseHTTP::_DELETE()
{
	//Check if the requested Resource is existing
	if (access(getFullRequestedPath().c_str(), F_OK) != 0)
		_createErrorResponse("/html/404.html", HTTP_404);

	//TBD: HOW TO CHECK THE ALLOWED METHODS HERE?

	//Check if file or directory is requested to delete
	PathType requestedResource = getPathType(getFullRequestedPath());
	if (requestedResource == PT_ERROR)
		throw std::runtime_error("DELETE: Could not determine file or dir type");
	else if (requestedResource == PT_FILE)
	{
		//check if the file has write rights
		if (access(getFullRequestedPath().c_str(), W_OK) != 0)
			return setResponseLine(HTTP_403, "Forbidden");
		else
		{
			if (std::remove(getFullRequestedPath().c_str()) == 0)
				return (setResponseLine(HTTP_200, "OK"));
			else
				throw std::runtime_error("DELETE: Could not delete file");
		}
	}
	else if (requestedResource == PT_DIR)
	{
		if (getFullRequestedPath()[getFullRequestedPath().length() - 1] != '/')
		{
			std::cout << "What should we do here? :)" << std::endl;
		}
		else
		{
			//check if the folder has write rights
			if (access(getFullRequestedPath().c_str(), W_OK) != 0)
				return setResponseLine(HTTP_403, "Forbidden");
			else
			{
				std::string command = "rm -rf " + getFullRequestedPath();
				if (std::system(command.c_str()) == 0)
					return (setResponseLine(HTTP_200, "OK"));
				else
					throw std::runtime_error("DELETE: Could not delete dir");
			}
		}
	}
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

std::string	ResponseHTTP::getFullRequestedPath() const
{
	return ("." + _request.getPath()); //TESTING!!! Change later. Depends where the root is mounted
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