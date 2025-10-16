#ifndef IAPICLIENT_H
#define IAPICLIENT_H
#include <string>

//create custom APIResponse struct
struct APIResponse{
    int status;
    std::string body;
};

//API methods
class IAPIClient {
    public:
        virtual ~IAPIClient() = default;
        virtual APIResponse get(const std::string& endpoint) = 0;
};

#endif