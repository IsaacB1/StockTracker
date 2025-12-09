#include <Arduino.h>
#include <WiFi.h>
#include "esp_http_client.h"
#include "config.hpp"
#include "IAPIClient.h"
#include "HttpLibWrap.h"
#include "AccountSubType.h"
#include <fstream>
#include "cJSON.h"
#include <memory>
#include "SPIFFS.h"
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "trading212_cert.h"
#include <mbedtls/base64.h>

#define FILE_NAME "/accountInfo.csv"
#define ITEM_SIZE 1


using cJson_ptr = std::unique_ptr<cJSON, decltype(&cJSON_Delete)>;

//default set to StocksISA account
HttpLibWrap::HttpLibWrap(){
    secureClient.setCACert(trading212_root_ca);
}

HttpLibWrap::HttpLibWrap(char test){
}

void HttpLibWrap::wifiSetup(){
    //initalise NVS - non volatile storage
    WiFi.mode(WIFI_STA);
    WiFi.begin(Config::WIFI_SSID, Config::WIFI_PASS);
    
    Serial.print("Connecting to WiFi");
    int timeout = 50;  // 10 seconds (50 * 200ms)
    while (WiFi.status() != WL_CONNECTED && timeout > 0) {
        delay(200);
        Serial.print(".");
        timeout--;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected!");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nConnection failed!");
    }

    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    
    // Wait for time to be set
    time_t now = time(nullptr);
    while (now < 1600000000) {  // Wait until time is reasonable
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
}

//ARGUMENTS -> Endpoint (place to reach) -> Buffer (what we store response in )
bool HttpLibWrap::get(const char* endpoint, char* response_buffer, size_t buffer_size) {
    
    char full_url[256];
    //snsprintf - safe string formattingn
    snprintf(full_url, sizeof(full_url), "%s%s", Config::API_HOST, endpoint);

    HTTPClient http;
    http.setConnectTimeout(30000);  
    // Begin connection with secure client
    if (!http.begin(secureClient, full_url)) {
        Serial.println("Failed to begin HTTP connection");
        return false;
    }
    
    // Set authorization header
    http.addHeader("Authorization", this->credentials);
    
    // Perform GET request
    Serial.println("Performing GET request...");
    int status = http.GET();
    //need to get the body in this 200
    Serial.printf("Response code: %d\n", status);
    switch (status){
        case 200: {
            //other statuses here
            //2mb buffer
            //check length of response
            //int len = esp_http_client_read(client_get, response_buffer, sizeof(response_buffer) -1);
            String response = http.getString();
            if(response.length() < buffer_size){
                strncpy(response_buffer, response.c_str(), buffer_size - 1);
                response_buffer[buffer_size - 1] = '\0';
                return true;
            }
            return false;
        }
        case 401: {
            Serial.println("Bad 401 Error: ");
            Serial.println(http.getString());
            return false;
        }
        
        default:
            //esp_http_client_cleanup(client_get);
            return false;
    }
}

bool HttpLibWrap::post(const char* endpoint, cJson_ptr& body, char* response_buffer){
    //need to add headers, post

    char full_url[256];
    //snsprintf - safe string formattingn
    snprintf(full_url, sizeof(full_url), "%s%s", Config::API_HOST, endpoint);

    char* body_str = cJSON_PrintUnformatted(body.get());
    if(!body){ return false;}

    //pointer type to the request
    esp_http_client_handle_t client_post = esp_http_client_init(&config_post);

    esp_http_client_set_url(client_post, full_url);
    esp_http_client_set_header(client_post, "Authorization", this->credentials);
    esp_http_client_set_post_field(client_post, body_str, strlen(body_str));

    esp_http_client_perform(client_post);
    
    int status = esp_http_client_get_status_code(client_post);

    //need to get the body in this 200
    switch (status){
        case 200: {
        //other statuses here
        //2mb buffer
        //check length of response
            int len = esp_http_client_read(client_post, response_buffer, sizeof(response_buffer) -1);
            if (len > 0){ 
                response_buffer[len] = '\0';
                esp_http_client_cleanup(client_post);
                return true;
            }else{
                //log error
                esp_http_client_cleanup(client_post);
                return false;
            }
        }
        default:
            esp_http_client_cleanup(client_post);
            return false;
    }
}

bool HttpLibWrap::updateAccountSubType(const AccountSubType& type) noexcept{
    char auth_plain[256];
    unsigned char base64_output[512];
    size_t olen = 0;

    switch(type){
        case AccountSubType::Stocks: 
            snprintf(this->credentials, sizeof(this->credentials), "%s:%s", Config::API_KEY_ID_STOCKS, Config::API_SECRET_STOCKS);
            break;
        case AccountSubType::StocksISA: 
            snprintf(auth_plain,sizeof(auth_plain),"%s:%s",Config::API_KEY_ID_STOCKSISA,Config::API_SECRET_STOCKSISA);
            break;
        default:
            Serial.println("No type found");
            return false;
    }

    int result = mbedtls_base64_encode((unsigned char*)base64_output,sizeof(base64_output),&olen,(unsigned char*)auth_plain,strlen(auth_plain));
            
    if (result != 0) {
        Serial.printf("Base64 encode failed: %d\n", result);
        return false;
    }
    base64_output[olen] = '\0';
    snprintf(this->credentials, sizeof(this->credentials), "Basic %s", base64_output);
    Serial.printf("Stocks credentials set: %s\n", this->credentials);

    return true;

}

bool HttpLibWrap::downloadCSV(const char* link){

    esp_http_client_config_t config_download = {
    .url = link,
    .method = HTTP_METHOD_GET,
    .timeout_ms = 5000,

    };
    //pointer type to the request
    esp_http_client_handle_t client_download = esp_http_client_init(&config_download);
    esp_http_client_open(client_download, 0);
    
    int file_size = esp_http_client_fetch_headers(client_download);

    //COULD CHANGE TO READIGN AND LOADING IN CHUNKS
    //ASSUMING ITS NOT VER BIG
    char buffer[2048];
    //read in file
    int len = esp_http_client_read(client_download, buffer, sizeof(buffer) -1);
    if (len > 0){ 
        buffer[len] = '\0';

        if (!SPIFFS.begin(true)) {
            Serial.println("Failed to mount SPIFFS");
            return false;
        }

        //open file
        File file = SPIFFS.open(FILE_NAME, FILE_WRITE);

        if(!file){Serial.println("Failed to open for writing"); return false;}

        //amount of bits written
        size_t written = file.write((uint8_t*)buffer, len);
        if (written != len) {return false;}
        file.close();

        esp_http_client_cleanup(client_download);
        esp_http_client_cleanup(client_download);
        return true;
    }else{
        //not enough space log
        esp_http_client_cleanup(client_download);
        esp_http_client_cleanup(client_download);
        return false;
    }


}
