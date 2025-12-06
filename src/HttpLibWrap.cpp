#include <Arduino.h>
#include "esp_http_client.h"
#include "config.hpp"
#include "IAPIClient.h"
#include "HttpLibWrap.h"
#include "AccountSubType.h"
#include <fstream>
#include "cJSON.h"
#include <memory>
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "SPIFFS.h"

#define FILE_NAME "/accountInfo.csv"
#define ITEM_SIZE 1


using cJson_ptr = std::unique_ptr<cJSON, decltype(&cJSON_Delete)>;


//need to add API secrets now
HttpLibWrap::HttpLibWrap(){
    //Set up WIFI
    this->wifiSetup();
}

HttpLibWrap::HttpLibWrap(char test){
}

void HttpLibWrap::wifiSetup(){
    //initalise NVS - non volatile storage
    nvs_flash_init();
    esp_netif_init(); //sets up TCP/IP stack
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); //standard WIFI config

    wifi_config_t config_wifi = {};
    memcpy(config_wifi.sta.ssid, Config::WIFI_SSID, sizeof(config_wifi.sta.ssid) - 1);
    memcpy(config_wifi.sta.password, Config::WIFI_PASS, sizeof(config_wifi.sta.password) - 1);

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &config_wifi);
    esp_wifi_start();
    esp_wifi_connect();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
}

//ARGUMENTS -> Endpoint (place to reach) -> Buffer (what we store response in )
bool HttpLibWrap::get(const char* endpoint, char* response_buffer) {

    char full_url[256];
    //snsprintf - safe string formattingn
    snprintf(full_url, sizeof(full_url), "%s%s", Config::API_HOST, endpoint);

    //pointer type to the request
    esp_http_client_handle_t client_get = esp_http_client_init(&config_get);

    esp_http_client_set_url(client_get, full_url);
    esp_http_client_set_header(client_get, "Authorization", this->credentials);

    esp_http_client_perform(client_get);
    
    int status = esp_http_client_get_status_code(client_get);

    //need to get the body in this 200
    switch (status){
        case 200: {
            //other statuses here
            //2mb buffer
            //check length of response
            int len = esp_http_client_read(client_get, response_buffer, sizeof(response_buffer) -1);
            if (len > 0){ 
                response_buffer[len] = '\0';
                esp_http_client_cleanup(client_get);
                return true;
            }else{
                //log error
                esp_http_client_cleanup(client_get);
                return false;
            }
        }
        
        default:
            esp_http_client_cleanup(client_get);
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
    switch(type){
        case AccountSubType::Stocks: {
            snprintf(this->credentials, sizeof(this->credentials), "%s:%s", Config::API_KEY_ID_STOCKS, Config::API_SECRET_STOCKS);
            return true;
        }
        default:
            snprintf(this->credentials, sizeof(this->credentials), "%s:%s", Config::API_KEY_ID_STOCKSISA, Config::API_SECRET_STOCKSISA);
            return true;
    }
    return false;

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
