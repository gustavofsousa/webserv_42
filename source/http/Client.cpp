#include "Client.hpp"

Client::Client(const Request &request, Response &response) : 
    _request(request), 
    _response(response),
    _statusCode(0),
    _isCGI(false)
{
    this->handleHTTPMethod();
}

Client::~Client()
{
}

void Client::handleHTTPMethod(void)
{
    std::string pagePath(this->fileRequested());

    std::cout << pagePath << std::endl;

	std::cout << "StatusCode: " << _statusCode << std::endl;
	std::cout << "Method: " << _request.getMethod() << std::endl; 

    if (_isCGI){
        //exec CGI
    }
    if(_request.getMethod() == "GET" && _statusCode < 400){
        this->_response.processFileForHTTPResponse(pagePath, this->_statusCode);
        this->_response.send();
        std::cout << "Estou no get" << std::endl;
    }
    else if(_request.getMethod() == "POST" && _statusCode < 400){
        _statusCode = 401;
        //implement error page 401
    }
    else if(_request.getMethod() == "DELETE" && _statusCode > 400){
        std::cout << "Delete" << std::endl;
    }
    else{
        this->_statusCode = 404;
        pagePath = "./static_pages/error/404.html";
        this->_response.processFileForHTTPResponse(pagePath, this->_statusCode);
        this->_response.send();
    }
}

std::string	Client::fileRequested(void)
{
	std::vector<std::string>::iterator	it00;
	std::vector<std::string>::iterator	it01;
	std::vector<std::string>			tmpVec00;
	std::vector<std::string>			tmpVec01;
	size_t	i;
	std::string							fileRequested;

	tmpVec00 = this->_request.getServerConf().getIndex();
	it00 = find(tmpVec00.begin(), tmpVec00.end(), this->_request.getRequestedInf());

    if(this->_request.getRequestedInf().find(".py") != std::string::npos){
        this->_statusCode = 200;
        this->_isCGI = true;
        return this->_request.getLocation() + this->_request.getRequestedInf();
    }
	if ((this->_request.getLocation() == this->_request.getServerConf().getRoot()) && \
		((it00 != tmpVec00.end()) || this->_request.getRequestedInf().empty()))
	{
		if (it00 != tmpVec00.end())
			fileRequested.append(".").append(this->_request.getLocation()).append(this->_request.getRequestedInf());
		else
			fileRequested.append(".").append(this->_request.getLocation()).append(tmpVec00[0]);
        this->_statusCode = 200;
	}
	else
	{
		i = 0;
		while (i < this->_request.getServerConf().getLocation().size())
		{
			tmpVec00 = this->_request.getServerConf().getLocation()[i].getIndex();
			it00 = find(tmpVec00.begin(), tmpVec00.end(), this->_request.getRequestedInf());
			tmpVec01 = this->_request.getServerConf().getLocation()[i].getMethods();
			it01 = find(tmpVec01.begin(), tmpVec01.end(), this->_request.getMethod());
			if ((it00 != tmpVec00.end()) && (it01 != tmpVec01.end()) &&\
				(this->_request.getServerConf().getLocation()[i].getPath() == this->_request.getLocation()))
			{
				fileRequested.append(".").append(this->_request.getLocation()).append(this->_request.getRequestedInf());
                this->_statusCode = 200;
				break ;
			}
			i++;
		}
	}
	if (fileRequested.empty())
    {
		fileRequested.append("./static_pages/error/404.html");
        this->_statusCode = 404;
    }
	if (Utils::getTypePath(fileRequested) != 1)
	{
		fileRequested.erase().append("./static_pages/error/500.html");
        this->_statusCode = 500;
	}

    if ((this->_request.getMethod().compare(0, 6, "DELETE") == 0) && \
        (this->_request.getQueryString().size() == 1))
    {
        std::map<std::string, std::string> tmpMap;
        std::map<std::string, std::string>::iterator itMap;
        tmpMap = this->_request.getQueryString();
        itMap = tmpMap.begin();
//        std::cout << "o tamanho de tmpMap é: " << tmpMap.size() << " first: " << itMap->first << " second: " << itMap->second << std::endl;
        this->buildDeleteFile(fileRequested, itMap->second);
    }
//    else
//    {
//        std::cout << "a requisição é do método: " << this->_request.getMethod() << std::endl;
//    }
	return (fileRequested);
}

void    Client::buildDeleteFile(const std::string & path, const std::string & idValue)
{
//    std::cout << "start | buildDeleteFile path: " << path << " id Value: " << idValue << std::endl;
	std::ifstream	ifs;
    std::ofstream   ofs;
	std::string		line;
	std::string		page;

	ifs.open(path.c_str());
	if (ifs.is_open())
	{
//        std::cout << "início do 1º if | arquivo: " << path << " está aberto coomo leitura" << std::endl;
		while(std::getline(ifs, line))
		{
            if (line.find(idValue, 0) != std::string::npos)
            {
        		while(std::getline(ifs, line))
		        {
                    if (line.find("</div>", 0) != std::string::npos)
                    {
                        std::getline(ifs, line);
                        std::getline(ifs, line);
                        break ;
                    }
                }
            }
            page += line.append("\n");
		}
		ifs.close();
//        std::cout << "Final  do 1º if | arquivo: " << path << " está aberto coomo leitura" << std::endl;
	}
//    std::cout << "Inserir o conteudo para o arquivo: " << path << std::endl;
    if (!page.empty())
    {
        ofs.open(path.c_str(), std::ios::out | std::ios::trunc);
        if (ofs.is_open())
        {
//            std::cout << "início do 2º if | arquivo: " << path << " está aberto coomo leitura" << std::endl;
            ofs << page;
            ofs.close();
//            std::cout << "Final  do 2º if | arquivo: " << path << " está aberto coomo leitura" << std::endl;
        }
//        else
//            std::cout << "Não abriu o truncat do arquivo: " << path << std::endl;
    }
//        std::cout << "end   | buildDeleteFile" << std::endl;
}