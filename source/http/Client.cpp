#include "Client.hpp"

Client::Client(const Request &request, Response &response) : 
    _request(request), 
    _response(response),
    _statusCode(0)
{
    this->getMethod();
}

Client::~Client()
{
}

std::string Client::getMethod(void)
{
//    std::string pagePath("./static_pages/index.html");
    std::string pagePath(this->fileRequested());

    // I need to receive information from the header
    std::ifstream file(pagePath.c_str());
    if (!file)
    {
        this->_response.httpMessage.append("HTTP/1.1 404 Not Found\r\n\r\n");
        return "";
    }

    // Here execute	methods or CGI
    std::ostringstream page;
    page << file.rdbuf();
    file.close();

    /* std::ostringstream resp;
    resp << "HTTP/1.1 200 OK\n";
    resp << "Content-Type: text/html\n";
    resp << "Content-Length: " << "19" << "\n";
    resp << "<h1>webserver</h1>\n"; */
    page.flush();
    std::string text;
    int lenPage = page.str().size();
    text.append("HTTP/1.1 200 OK\r\n");
    text.append("Content-Type: text/html\r\n");
    std::string content_len;
    std::stringstream sstream;
    sstream << lenPage;
    content_len.append("Content-Length: ").append(sstream.str()); //para fazer funcinar na 42!
//    std::string content_len = "Content-Length: " + lenPage; // na 42, estava dando erro!
    content_len += "\r\n\n";
    text.append(content_len);
    text.append(page.str());
    /* text.append("<html>\n");
    text.append("<body>\n");
    text.append("<h1>Hello, World!</h1>\n");
    text.append("</body>\n");
    text.append("</html>\n" );*/
    // std::cout << "Response client: \n" << text << "\n\n";
    this->_response.httpMessage = text;
    _statusCode = 200;
    return (this->_response.httpMessage);
}

int Client::getStatusCode(void){
    return this->_statusCode;
}

std::string Client::readFile(std::string name){
    
    std::ifstream file(name.c_str());

    if (file.is_open()){
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string contents = buffer.str();

        std::cout << contents << "\n";
        file.close();
        _statusCode = 200;
        return contents;
    } else {
        _statusCode = 404;
        return "404 Not Found";
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