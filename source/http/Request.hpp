#pragma	once

# include "RequestParser.hpp"
# include <fstream>
# include <string>
# include <iostream> //confirmar se será necessário para a entrega
# include <sstream>
# include <map>
# include "./../config/Error.hpp"
# include "../Utils.hpp"
//# include "./../config/Utils.hpp"
# include <sys/socket.h>
# include <string.h>
# include <string> //to_string()
# include <algorithm> //for search
# include "../config/ConfigFile.hpp"

# define BUFFER_SIZE 4096

class RequestParser;

struct requestInfo
{
		size_t								contentLength; // inicializa com -1.
		std::string							method;
		std::string							location;
		std::string							requestedInf;
		std::string							contentType;
		std::map<std::string, std::string>	queryString;
		std::string							queryStringS;
		std::string							userAgent;
		std::string							host;
};


class	Request
{
	public:
		Request(int newClient, ConfigFile _configFile);
		Request 			&operator=(Request const & other);
		Request(Request const & other);
		~Request(void);


		//methods
		bool		            receiveFromClient(int client);
        bool                    isReady();
        void                    reset();
        void                    clearAll();

		//getters
		const std::string &		getMethod(void) const;
		const std::string &		getLocation(void) const;
		const std::string &		getRequestedInf(void) const;
		const std::string &		getUserAgent(void) const;
		const std::string &		getHost(void) const;
		const std::string &		getContentType(void) const;
        int                     getContentLength();
		const std::string &		getQueryStringS(void) const;
		const std::map<std::string, std::string> &	getQueryString(void) const;

		const ConfigFile  &		getServerConf(void) const;
		const std::string &		getHeader(void) const;
		const std::string &		getBody(void) const;

		// Setters
		void					setMethod(std::string newMethod);
		void					setLocation(std::string newLocation);
		void					setRequestedInf(std::string newInfo);
		void					setUserAgent(std::string newAgent);
		void					setHost(std::string newHost);
		void					setContentType(std::string newContentType);
		void					setQueryStringS(std::string newQueryStringS);
		void					setQueryString(std::string key, std::string value);
		void					setContentLength(int value);

	private:
        bool                        _ready;
		std::string					_header;
        std::string			        _body;
        std::string                 _delimeter;
		std::string					_httpMessage;
        int                         _fromClient; // comes from constuctor. It is the socket of server.
    	ConfigFile					_serverConf;
		struct requestInfo			_info;
		RequestParser				_parser;

        int                     checkBytesReceived(ssize_t bytes_received);
		int			            extractHeader(std::string const& buffer );
		int			            extractBody(std::string const& buffer, int bytes);
        bool                    appendTheBody(std::string const& buffer, int bytes);

};
