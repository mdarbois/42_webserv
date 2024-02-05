/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:41:03 by aehrlich          #+#    #+#             */
/*   Updated: 2024/02/05 20:08:36 by aehrlich         ###   ########.fr       */
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
  	_path = _config.getLocationPath(_request.getPath());
	_pathRoot = _config.getLocationRoot(_path, _request.getPath());
	std::cout << "PATHROOT: " << _pathRoot << std::endl;
	/* 
	std::cout << "request path=" << _request.getPath() << std::endl;
	std::cout << "PATH=" << _path << std::endl;
	std::map<std::string, LocationConfig> locationCopy (_config.getLocations());
	std::map<std::string, LocationConfig>::const_iterator it;
    for (it = locationCopy.begin(); it != locationCopy.end(); ++it) {
			std::cout << "it->first : " << it->first << std::endl;
        if (it->first == _path) {
            break; // Key found, exit loop
        }
    }
    // Check if key was found
    if (it != locationCopy.end() && (it->second).getAutoindex() == true && endsWithSlash(_request.getPath()))
	{
		//std::cout << "it->second : " << it->second << std::endl;
		std::cout << "AUTOINDEX" << std::endl;
	} */
 	if (_checkRed())
		return;
	if (request.getBody().size() > config.getClientMaxBodySize())
	{
		_createErrorResponse("/413.html", HTTP_413); //TODO Paths are still weird
		return;
	}
	//Very basic. A lot of cheecks have to be performed
	if (request.getMethod() == GET)
		_GET();
	else if (request.getMethod() == POST)
		_POST();
	else if (request.getMethod() == DELETE)
		_DELETE();
	else
		_createErrorResponse("/403.html", HTTP_403); //TODO Paths are still weird
}

ResponseHTTP::ResponseHTTP(const CGI& cgi, ServerConfig config)
{
	_path = _config.getLocationPath(_request.getPath());
	_pathRoot = _config.getLocationRoot(_path, _request.getPath());
	if (!config.getHost().empty())
		_body = cgi.getBody();
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
	_request.overidePath(errPagePath); //TODO This has to be merged with your PATH matching @MArie
	_readFile();
	setResponseLine(status);
}

bool	ResponseHTTP::_readFile()
{
	std::ifstream	htmlFile(getFullRequestedPath().c_str());
	std::cout << getFullRequestedPath().c_str() << std::endl;
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

bool ResponseHTTP::_checkRed()
{

  std::map<std::string, LocationConfig> locations(_config.getLocations());
  for (std::map<std::string, LocationConfig>::const_iterator it = locations.begin(); it != locations.end(); ++it)
  if(it->first == _path && !(it->second.getRedirection().empty()))
  {
      std::string statusCode = (it->second).getRedirection().substr(0,3);
			setResponseLine(static_cast<HttpStatus>(std::atoi(statusCode.c_str())));
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
      return true;
  }
  return false;

}

void	ResponseHTTP::_GET()
{
	//check redirection
		//fetch the redirection folders from serverconfig
	/* 	std::string pathNoRoot = _request.getPath().erase(0,5);
		//pathNoRoot.erase(pathNoRoot.length() - 1);
		//char lastChar = pathNoRoot[pathNoRoot.length() - 1];
		 if (lastChar == '/')
		{ 
			std::vector<std::string> tokens = splitString(pathNoRoot, '/');
			prependCharacter(tokens, '/');
			for (size_t i = 0; i < tokens.size(); ++i)
			{
				std::cout << "token = " << tokens[i] << std::endl;
				std::map<std::string, LocationConfig> locations(_config.getLocations());
				for (std::map<std::string, LocationConfig>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
				std::cout << "locations = " << it->first << std::endl;
				if(it->first == tokens[i] && !(it->second.getRedirection().empty()))
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
			}
		//}
		std::cout << "path=" <<  pathNoRoot << std::endl; */
		//_config.getLocations()[]


	// check if _request.getPath() ends with /
	// if so, directory: check if index, check if autoindex
	// if not, file: check access



	// Check redirection
	/* 
	std::map<std::string, LocationConfig> locations(_config.getLocations());
	for (std::map<std::string, LocationConfig>::const_iterator it = locations.begin(); it != locations.end(); ++it) {

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
	} */
	std::cout << "_______" << std::endl;
	std::cout << _request.getPath() << std::endl;
	std::cout << _pathRoot << std::endl;
	std::cout << _path << std::endl;
	std::cout << "_______" << std::endl;
	 if (_pathRoot[_pathRoot.length() - 1] == '/')
	{
		std::cout << "AUTOINDEX" << std::endl;
	} 
	//Check if the requested Resource is existing
	//if (access(getFullRequestedPath().c_str(), F_OK) != 0)
		//return (_createErrorResponse("/404.html", HTTP_404));


	//TODO: HOW TO CHECK THE ALLOWED METHODS HERE?
	std::cout << _config.getLocations()[_path] << std::endl;
	if (!containsValue<std::string>(_config.getLocations()[_path].getMethods(), "GET"))
		return (_createErrorResponse("/403.html", HTTP_403));
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
			setResponseLine(HTTP_200);
	}
	else if (requestedResource == PT_DIR)
	{
		if (getFullRequestedPath()[getFullRequestedPath().length() - 1] != '/')
		{
			std::cout << "What should we do here? :)" << std::endl;
		}
		else
		{
			if (!_config.getLocations()[_path].getIndex().empty())
			{
				//override the path/
				_request.overidePath(_config.getLocations()[_request.getPath()].getIndex());
				std::cout << _config.getRoot() << std::endl;
				std::cout << _request.getPath() << std::endl;
				if (!_readFile())
					throw std::runtime_error("GET: Could not open file");
				else
					setResponseLine(HTTP_200);
			}
			else
			{
				if (!_config.getLocations()[_request.getPath()].getAutoindex())
				{
					//No index and no auto index
					//return the error page of this location or the default error page
					//Error 403
					_createErrorResponse("/403.html", HTTP_403);
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
		return _createErrorResponse("/400.html", HTTP_400); //TODO PATH

	//check if the folder is accessible and the rights to post a file
	if (access(getFullRequestedPath().c_str(), F_OK | W_OK) != 0)
		return _createErrorResponse("/404.html", HTTP_404); //TODO PATH
		
	//check if the location supports POST
	if (!containsValue<std::string>(_config.getLocations()[_path].getMethods(), "POST"))
		return _createErrorResponse("/403.html", HTTP_403); //TODO PATH

	//create a new file
	std::string	uploadFilePath = std::string(_config.getRoot() + _request.getPath()) + _request.getUploadFilename();
	std::ofstream	newFile(uploadFilePath.c_str());
	if (!newFile.is_open())
		throw std::runtime_error("Could not create posted file");
	setResponseLine(HTTP_201);
	setHeader("Access-Control-Allow-Origin", "*");
	size_t endHeader = _request.getBody().find("\r\n\r\n") + 4;
	size_t endFormData = _request.getBody().find(_request.getEndBoundary());
	newFile << _request.getBody().substr(endHeader, endFormData - endHeader - 2);
	newFile.close();
}

void	ResponseHTTP::_DELETE()
{
	//Check if the requested Resource is existing
	if (access(getFullRequestedPath().c_str(), F_OK) != 0)
		return (_createErrorResponse("/404.html", HTTP_404));

	//check if the location supports POST
	std::cout << _pathRoot << std::endl;
	if (!containsValue<std::string>(_config.getLocations()[_path].getMethods(), "DELETE"))
		return _createErrorResponse("/403.html", HTTP_403); //TODO PATH

	//Check if file or directory is requested to delete
	PathType requestedResource = getPathType(getFullRequestedPath());
	if (requestedResource == PT_ERROR)
		throw std::runtime_error("DELETE: Could not determine file or dir type");
	else if (requestedResource == PT_FILE)
	{
		//check if the file has write rights
		if (access(getFullRequestedPath().c_str(), W_OK) != 0)
			return _createErrorResponse("/403.hmtl", HTTP_403);
		else
		{
			if (std::remove(getFullRequestedPath().c_str()) == 0)
				return (setResponseLine(HTTP_200));
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
				return _createErrorResponse("/403.hmtl", HTTP_403);
			else
			{
				std::string command = "rm -rf " + getFullRequestedPath();
				if (std::system(command.c_str()) == 0) //TODO should we delete folders???
					return (setResponseLine(HTTP_200));
				else
					throw std::runtime_error("DELETE: Could not delete dir");
			}
		}
	}
}

std::string ResponseHTTP::_getResponsePhrase(HttpStatus status) const
{
	switch (status)
	{
	case 200:
		return "OK";
		break;
	case 201:
		return "Created";
		break;
	case 301:
		return "Moved Permanently";
		break;
	case 400:
		return "Bad Request";
		break;
	case 403:
		return "Forbidden";
		break;
	case 404:
		return "Not Found";
		break;
	case 405:
		return "Method Not Allowed";
		break;
	case 408:
		return "Request Timeout";
		break;
	case 409:
		return "Conflict";
		break;
	case 413:
		return "Payload too large";
		break;
	case 500:
		return "Internal server error";
		break;
	case 505:
		return "HTTP Version not supported";
		break;
	default:
		return "HTTP Error";
		break;
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
	return ( _config.getRoot() + _request.getPath()); //TODO TESTING!!! Change later. Depends where the root is mounted
}

void	ResponseHTTP::setBody(std::string body)
{
	_body = body;
}

void	ResponseHTTP::setResponseLine(HttpStatus status)
{
	_responseLine.allowedHttpProtocol = ALLOWED_HTTP_PROTOCOL;
	_responseLine.httpStatusCode = status;
	_responseLine.reasonPhrase = _getResponsePhrase(status);
}

void	ResponseHTTP::setHeader(std::string key, std::string value)
{
	_header[key] = value;
}

/* ************************************************************************** */
