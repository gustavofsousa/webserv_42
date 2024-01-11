#pragma	once

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
# include "../core/Server.hpp"

# define BUFFER_SIZE 4096

class	Request
{
	public:
		Request(void);
		Request(int newClient);
		Request(struct clientInfo clientInfo, ConfigFile _configFile);
//		Request(int newClient, const ConfigFile & confFile);
		// Request(Request const& copy);
		~Request(void);

		bool		            receiveFromClient(int client);
		void		            parseRequest();
		const std::string &		getMethod(void) const;
		const std::string &		getLocation(void) const;
		const std::string &		getRequestedInf(void) const;
		const std::string &		getContentType(void) const;
		const ConfigFile  &		getServerConf(void) const;
        bool                    isReady();
        void                    reset();
        void                    clearAll();
        int                     totalLength();
		const std::map<std::string, std::string> &	getQueryString(void) const;
		const std::string &		getQueryStringS(void) const;

	private:
        int                                 _fromClient;
		int									_ipClient;
		int									_portClient;
        bool                                _ready;
		size_t								_contentLength;
		std::string							_header;
        std::string     			        _body;
		std::string							_httpMessage;
		std::string							_method;
		std::string							_location;
		std::string							_requestedInf;
		std::string							_contentType;
		std::map<std::string, std::string>	_queryString;
        std::string                         _delimeter;
    	ConfigFile							_serverConf;
		std::string							_queryStringS;

		void		            splitRequest(std::string & fullRequest, size_t & pos);
		void		            parseQueryString(std::string queryString);
		std::string	            urlDecoder(const std::string & url);
        int                     checkBytesReceived(ssize_t bytes_received);
		int			            getHeader(std::string const& buffer );
		int			            getBody(std::string const& buffer, int bytes);
		bool		            getContentLength();
        bool                    appendTheBody(std::string const& buffer, int bytes);

};
