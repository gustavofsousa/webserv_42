#pragma	once

# include <fstream>
# include <string>
# include <iostream> //confirmar se será necessário para a entrega
# include "./../config/Error.hpp"
# include "./../config/Utils.hpp"

class	Request
{
	private:
		std::string	_method;
		std::string	_location;
		std::string	_requestedInf;
		std::string	_contentType;
		Request(const Request& copy);
		Request	&operator=(const Request &src);

		void	splitRequest(std::string & fullRequest, size_t & pos);

	public:
		Request(std::string buffer);
		~Request(void);

		const std::string &		getMethod(void) const;
		const std::string &		getLocation(void) const;
		const std::string &		getRequestedInf(void) const;
		const std::string &		getContentType(void) const;

		void					parserRequest(std::string & request);
};
