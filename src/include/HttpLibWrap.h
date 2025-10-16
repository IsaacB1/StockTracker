#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "IAPIClient.h"
#ifndef HTTPLIBWRAP_H
#define HTTPLIBWRAP_H

//class used to implement httplib.h's methods for our uses.
//also implements IAPIClient interface
class HttpLibWrap : public IAPIClient{
    private:
        httplib::SSLClient cli;
        std::string accountInfo = "/equity/account/cash";

    public:
        HttpLibWrap(const std::string& host);
        ~HttpLibWrap() = default;
        APIResponse get(const std::string& endpoint);
        //wont be implemented but could be used to connet to my server to store data
};

#endif