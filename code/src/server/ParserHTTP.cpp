#include "../../includes/ParserHTTP.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ParserHTTP::ParserHTTP(std::string requestString)
{
	_requestString = requestString;
	_method = NONE;
	_path = "";
	_httpProtocol = "";
	_isCGI = false;
	_isUpload = false;
	parseRequest();
	std::cout << *this;
}

/* ParserHTTP::ParserHTTP()
{
} */

/* ParserHTTP::ParserHTTP( const ParserHTTP & src )
{
} */


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ParserHTTP::~ParserHTTP()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/* ParserHTTP &				ParserHTTP::operator=( ParserHTTP const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
} */

std::ostream &			operator<<( std::ostream & o, ParserHTTP const & i )
{
	o << "----------------------------PARSED HTTP REQUEST----------------------------" << std::endl;
	o << "Request Line:" << std::endl;
	o << "\t-Method: " << i.getMethodString() << std::endl;
	o << "\t-URI: " << i.getPath() << std::endl;
	o << "\t-Protocol: " << i.getProtocol() << std::endl << std::endl;
	o << "Header:" << std::endl;
	std::map<std::string, std::string> headers = i.getHeaderMap();
	std::map<std::string, std::string>::iterator it;
	for (it = headers.begin(); it != headers.end(); ++it) {
		o << "\t-" << it->first << ": " << it->second << std::endl;
	}
	o << std::endl;
	o << "Body:";
	if (i.getBody().empty())
		o << "\t" << "*No Request body*" << std::endl;
	o << std::endl << i.getBody() << std::endl << std::endl;
	if (i.getCGIStatus())
		o << "CGI:\tTRUE" << std::endl;
	else
		o << "CGI:\tFALSE" << std::endl;
	if (i.getUploadStatus())
		o << "Upload:\tTRUE" << std::endl;
	else
		o << "Upload:\tFALSE" << std::endl;
	o << "----------------------------------------------------------------------------" << std::endl;
	o << std::endl;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

std::string trimChars(const std::string& input, const std::string& charsToTrim) {
	size_t start = input.find_first_not_of(charsToTrim);
	size_t end = input.find_last_not_of(charsToTrim);

	if (start == std::string::npos || end == std::string::npos)
		return "";
	return input.substr(start, end - start + 1);
}

void	ParserHTTP::parseRequest()
{
	std::string	line;
	std::stringstream	requestStream(_requestString);
	std::getline(requestStream, line);
	std::stringstream	headline(line);
	std::string	method;
	headline >> method >> _path >> _httpProtocol;
	if (method == "GET")
		_method = GET;
	else if(method == "POST")
		_method = POST;
	else if (method == "DELETE")
		_method = DELETE;
	else
		_method = NONE;
	if (_path.length() > std::strlen(".py"))
		if (_path.substr(_path.length() - 3) == ".py")
			_isCGI = true;
	trimChars(_path, "\n\r ");
	trimChars(_httpProtocol, "\n\r ");
	while (std::getline(requestStream, line) && line != "\r") {
		size_t colonPos = line.find(':');
		if (colonPos != std::string::npos) {
			std::string headerName = line.substr(0, colonPos);
			std::string headerValue = line.substr(colonPos + 1);
			trimChars(headerValue, "\n\r ");
			_header[headerName] = headerValue;
			if (headerValue.find("multipart/form-data") != std::string::npos)
				_isUpload = true;
		}
	}
	std::getline(requestStream, _body, '\0');
	trimChars(_body, "\n\r ");
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
HttpMethod	ParserHTTP::getMethod() const
{
	return (_method);
}

std::string	ParserHTTP::getMethodString() const
{
	if (_method == GET)
		return "GET";
	else if (_method == POST)
		return "POST";
	else
		return "DELETE";
}

std::string	ParserHTTP::getPath() const
{
	return (_path);
}

std::string	ParserHTTP::getProtocol() const
{
	return (_httpProtocol);
}

bool	ParserHTTP::getCGIStatus() const
{
	return (_isCGI);
}

bool	ParserHTTP::getUploadStatus() const
{
	return (_isUpload);
}

std::string	ParserHTTP::getBody() const
{
	return (_body);
}

std::map<std::string, std::string>	ParserHTTP::getHeaderMap() const
{
	return (_header);
}

/* ************************************************************************** */