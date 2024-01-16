#pragma once
#include "Request.hpp"

# include <string>
class Request;


class RequestParser
{
	public:
		RequestParser( Request & request);
        RequestParser& operator=(RequestParser const & other);
        RequestParser(RequestParser const & other);
		~RequestParser( void );
		bool					parse( );

	private:
		Request &							_request;
		std::string							_header;
        struct requestInfo					_info;

		void		            splitRequest(size_t & pos);
		void		            parseQueryString(std::string queryString);
		std::string	            urlDecoder(const std::string & url);
		bool		            extractContentLength();
		std::string				extractInFor(const std::string keyword);
};