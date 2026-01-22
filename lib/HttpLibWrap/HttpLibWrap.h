#include "IAPIClient.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "config.hpp"
#include <esp_crt_bundle.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#pragma once

using cJson_ptr = std::unique_ptr<cJSON, decltype(&cJSON_Delete)>;

inline esp_http_client_config_t config_get = {
    .url = Config::API_HOST,
    .method = HTTP_METHOD_GET,
    .timeout_ms = 5000,
    .crt_bundle_attach = arduino_esp_crt_bundle_attach,
};

inline esp_http_client_config_t config_post = {
    .url = Config::API_HOST,
    .method = HTTP_METHOD_POST,
    .timeout_ms = 100000,
    .transport_type = HTTP_TRANSPORT_OVER_SSL,
    .crt_bundle_attach = arduino_esp_crt_bundle_attach,
};

//class used to implement httplib.h's methods for our uses.
//also implements IAPIClient interface
class HttpLibWrap : public IAPIClient{
    private:
        char credentials[256];
        WiFiClientSecure secureClient;
    public:
        HttpLibWrap();
        HttpLibWrap(char test);
        ~HttpLibWrap() = default;
        void wifiSetup();
        bool get(const char* endpoint, char* response_buffer, size_t buffer_size) override;
        bool getLink(const char* link, char* response_buffer, size_t buffer_size) override;
        bool get(const char* endpoint, char* response_buffer, size_t buffer_size, char* report_id);
        bool post(const char* endpoint, cJson_ptr& body, char* response_buffer, size_t buffer_size) override;
        //wont be implemented but could be used to connet to my server to store data
        bool updateAccountSubType(const AccountSubType& newType) noexcept;
        bool downloadCSV(const char* link);
};      

