/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserHTTP.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 09:24:12 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/25 14:22:17 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSERHTTP_HPP
# define PARSERHTTP_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <map>
# include <cstring>
# include <string>

typedef enum e_httpMethod
{
	GET,
	POST,
	DELETE,
	NONE
}HttpMethod;

class ParserHTTP
{

	public:
		ParserHTTP(std::string requestString);
		~ParserHTTP();
		ParserHTTP();
		ParserHTTP( ParserHTTP const & src );
		ParserHTTP &		operator=( ParserHTTP const & rhs );
		
		void	parseRequest();
		HttpMethod							getMethod() const;
		std::string							getMethodString() const;
		std::string							getPath() const;
		std::string							getProtocol() const;
		bool								isCGI() const;
		bool								isUpload() const;
		std::string							getBody() const;
		std::string							getUploadFilename() const;
		std::string							getStartBoundary() const;
		std::string							getEndBoundary() const;
		std::map<std::string, std::string>	getHeaderMap() const;
		std::map<std::string, std::string>	getCGIParamMap() const;

		void								overrideReqPathtoErrorPath(std::string errorPath);
		
	private:
		std::string							_requestString;
		HttpMethod							_method;
		std::string							_path;
		std::string							_httpProtocol;
		bool								_isCGI;
		bool								_isUpload;
		std::string							_body;
		std::map<std::string, std::string>	_header;
		std::map<std::string, std::string>	_cgiParams;
		std::string							_uploadFilename;
		std::string							_startBoundary;
		std::string							_endBoundary;
};

std::ostream &			operator<<( std::ostream & o, ParserHTTP const & i );

#endif /* ****************************************************** PARSERHTTP_H */