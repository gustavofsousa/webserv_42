#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

class   HTTPRequest {
    public:
        HTTPRequest();
        ~HTTPRequest;

        std::string const&  _method;
}

#endif