#ifndef IAPICLIENT_H
#define IAPICLIENT_H
#include "AccountSubType.h"
#include "cJSON.h"
#include <memory>

using cJson_ptr = std::unique_ptr<cJSON, decltype(&cJSON_Delete)>;


//API methods
class IAPIClient {
    public:
        virtual ~IAPIClient() = default;
        virtual bool get(const char* endpoint, char* response_buffer, size_t buffer_size) = 0;
        virtual bool post(const char* endpoint, cJson_ptr& body, char* response_buffer) = 0;
};

#endif