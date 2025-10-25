#ifndef IAPICLIENT_H
#define IAPICLIENT_H
#include "AccountSubType.h"
#include <string>
#include <json.hpp>

using json = nlohmann::json;

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
        virtual APIResponse post(const   std::string_view& endpoint, const json body) = 0;
        virtual void updateAccountSubType(const AccountSubType& newType) noexcept = 0;
};

#endif