#ifndef ResponseHTTP_HPP
# define ResponseHTTP_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <map>
#include <vector>
#include <algorithm>
# include "utils.hpp"
# include "ParserHTTP.hpp"
# include "Config.hpp"
# include "Socket.hpp" //because of HTTP Codes -> has to be done cleaner

# define ALLOWED_HTTP_PROTOCOL "HTTP/1.1"
typedef struct s_ResponseLine
{
	std::string	allowedHttpProtocol;
	HttpStatus	httpStatusCode;
	std::string	reasonPhrase;
}ResponseLine;


class ResponseHTTP
{
	public:
		ResponseHTTP(ParserHTTP	request, ServerConfig config);
		~ResponseHTTP();
		ResponseHTTP &		operator=( ResponseHTTP const & rhs );
		ResponseHTTP();
		ResponseHTTP( ResponseHTTP const & src );

		std::string							getFullResponseString() const;
		std::map<std::string, std::string>	getResponseHeaderMap() const;
		int									getBodyLength() const;
		int									getResponseLength() const;

		void								setBody(std::string body);
		void								setResponseLine(HttpStatus status, std::string reasonPhrase);
		void								setHeader(std::string key, std::string value);

	private:
		ServerConfig						_config;
		ResponseLine						_responseLine;
		std::string							_body;
		std::map<std::string, std::string>	_header;
		ParserHTTP							_request;
		int									_bodyLength;
		bool								_readFile();
		void								_createErrorResponse();
		bool								_isValidRequest();
		void								_GET();
		void								_POST();
		void								_DELETE();
};

std::ostream &			operator<<( std::ostream & o, ResponseHTTP const & i );

#endif /* **************************************************** ResponseHTTP_H */