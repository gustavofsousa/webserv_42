#pragma	once

# include <fstream>
# include <string>
# include <iostream> //confirmar se será necessário para a entrega
# include <sstream>
# include <map>
# include "./../config/Error.hpp"
# include "./../config/Utils.hpp"
# include <sys/socket.h>
# include <string.h>
# include <string> //to_string()
# include <algorithm> //for search

# define BUFFER_SIZE 4096

class	Request
{
	public:
		Request(void);
		~Request(void);

		int			            receiveFromClient(int client);
		void		            parseRequest();
		const std::string &		getMethod(void) const;
		const std::string &		getLocation(void) const;
		const std::string &		getRequestedInf(void) const;
		const std::string &		getContentType(void) const;
		const std::map<std::string, std::string> &	getQueryString(void) const;

	private:
		size_t									_contentLength;
		std::string							_header;
        std::string     			        _body;
		std::string							_httpMessage;
		std::string							_method;
		std::string							_location;
		std::string							_requestedInf;
		std::string							_contentType;
		std::map<std::string, std::string>	_queryString;
        bool                                _ready;
        std::string                         _delimeter;

		Request(const Request& copy);
		Request	                &operator=(const Request &src);
		void		            splitRequest(std::string & fullRequest, size_t & pos);
		void		            parseQueryString(std::string queryString);
		std::string	            urlDecoder(const std::string & url);
        int                     checkBytesReceived(ssize_t bytes_received);
		int			            getHeader(int client );
		int			            getBody(int client);
		void		            getContentLength();
        bool                    appendFirstBody(std::string buffer);

};
