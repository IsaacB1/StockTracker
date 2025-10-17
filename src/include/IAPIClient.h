#ifndef IAPICLIENT_H
#define IAPICLIENT_H
#include "AccountSubType.h"
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
        virtual APIResponse get(const   std::string_view& endpoint) = 0;
        virtual void updateAccountSubType(const AccountSubType& newType) noexcept = 0;
};

#endif