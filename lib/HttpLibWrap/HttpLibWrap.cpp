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
    secureClient.setCACert(trading212_ca_bundle);

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

bool readDownloadLink(WiFiClient* stream, char* download_link_buffer, size_t download_link_buffer_size){
    //read in chars in buffer -> bit by bit
    char chunk[512] = {0}; //sets every byte to 0
    size_t latest_len = 0; //for size meaning we can check if latest_len = 0 then hasnt been done corectly

    const char key[] = "\"downloadLink\":\"";
    const size_t key_len = sizeof(key) - 1;

    //for matching the stream with downlaod link
    size_t match = 0; //Number of characters weve matched so far 
    bool capturing = false;

    while(stream->connected() && stream->available()){
        //reading one Byte at a time -> due to network splitting packets
        char c;
        //tryes to read N Bytes from stream -> save to char c 
        //-> if N bytes read == to Bytes we wanted to read then GOOD
        if (stream->readBytes(&c, 1) != 1) continue;

        if(!capturing){
            //if byte weve just got == to the index of the key we are checking against 
            if (c == key[match]){
                //success so 
                match++;
                //check if weve completed the whole thing
                if(match == key_len){
                    capturing = true;
                    latest_len = 0;
                }
            }else{
                if(c == key[0]){
                    match = 1;
                }else{
                    match = 0;
                }
            }
        }else{
            //this is once capturing == true
            //if at end of link
            if(c == '"'){
                chunk[latest_len] = '\0';
                capturing = false;
                Serial.println(chunk);
            }else if(latest_len < sizeof(chunk) - 1){
                chunk[latest_len] = c;
                latest_len++;
            }
        }
    }
    Serial.println("Outside While loop");
    if(latest_len < download_link_buffer_size){
        strncpy(download_link_buffer, chunk, download_link_buffer_size - 1);
        return true;
    }else{
        Serial.println("Need to set tempoary chunk and download_link_buffer to be same size please");
        return false;
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

            /*
            Serial.println(buffer_size);
            Serial.println(response.length());
            Serial.println(response);
            */

            if(response.length() < buffer_size){
                strncpy(response_buffer, response.c_str(), buffer_size - 1);
                response_buffer[buffer_size - 1] = '\0';
                return true;
            }else{
                //Serial.println(response);
                //Serial.println(response.length());
                //Serial.println(buffer_size);
                Serial.println("Failed to copy response into buffer - response too large");
                return false;
            }
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

//ARGUMENTS -> Link (place to reach) -> Buffer (what we store response in )
bool HttpLibWrap::getLink(const char* link, char* response_buffer, size_t buffer_size) {


    HTTPClient http;
    http.setConnectTimeout(30000);  
    // Begin connection with secure client
    if (!http.begin(secureClient, link)) {
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
            WiFiClient* stream = http.getStreamPtr();
            //turn stream into file now
        }
        case 401: {
            Serial.println("Bad 401 Error: ");
            return false;
        }
        default:
            return false;
    }
}

//ARGUMENTS -> Endpoint (place to reach) -> Buffer (what we store response in ) -> report id for getting downlaod link of CSV generated, overrided get for filtering reportid client side
bool HttpLibWrap::get(const char* endpoint, char* response_buffer, size_t buffer_size, char* report_id) {
    
    //make sure arguments are safe 
    if (!endpoint || !response_buffer || buffer_size == 0 || !report_id) {
        Serial.println("Invalid arguments");
        return false;
    }

    static char full_url[256];

    //snsprintf - safe string formattingn - not safe alwyas apparantley 
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

    Serial.printf("Free heap before: %d bytes\n", ESP.getFreeHeap());
    //CRASHING HERE CHECK
    int status = http.GET();
    //need to get the body in this 200
    Serial.printf("Response code: %d\n", status);
    switch (status){
        case 200: {
            //other statuses here
            //2mb buffer
            //check length of response
            //int len = esp_http_client_read(client_get, response_buffer, sizeof(response_buffer) -1);
            
            /*
            Serial.println(buffer_size);
            Serial.println(response.length());
            Serial.println(response);
            */

            WiFiClient* stream = http.getStreamPtr();

            const size_t download_link_buffer_size = 512;
            char download_link_buffer[download_link_buffer_size];

            if (readDownloadLink(stream, response_buffer, buffer_size)){
                Serial.println("Here");
                return true;
            }else{
                return false;
            }
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

bool HttpLibWrap::post(const char* endpoint, cJson_ptr& body, char* response_buffer, size_t buffer_size){
    //need to add headers, post

    char full_url[256];
    //snsprintf - safe string formattingn
    snprintf(full_url, sizeof(full_url), "%s%s", Config::API_HOST, endpoint);

    Serial.println("Full Url constructed");
    HTTPClient http;
    http.setConnectTimeout(30000);  
    // Begin connection with secure client
    if (!http.begin(secureClient, full_url)) {
        Serial.println("Failed to begin HTTP connection");
        return false;
    }
    
    // Set authorization header
    http.addHeader("Authorization", this->credentials);
    
    //this needs to change - needs to be configurable 
    http.addHeader("Content-Type", "application/json");

    // Perform POST request
    Serial.println("Performing POST request...");
    
    char* payload = cJSON_PrintUnformatted(body.get());
    if (!payload) {
        Serial.println("Invalid payload");
        return false;
    }
    Serial.printf("%s\n", payload);
    Serial.printf("Payload length: %d\n", strlen(payload));
    Serial.printf("Free heap before: %d bytes\n", ESP.getFreeHeap());
    int status = http.POST((uint8_t*)payload, strlen(payload));
    delay(1000);
    cJSON_free(payload);
    //need to get the body in this 200
    Serial.printf("Response code: %d\n", status);
    switch (status){
        case 200: {
            //other statuses here
            //2mb buffer
            //check length of response
            String response = http.getString();
            Serial.println(response);
            if(response.length() < buffer_size){
                strncpy(response_buffer, response.c_str(), buffer_size - 1);
                response_buffer[buffer_size - 1] = '\0';
                return true;
            }
            Serial.println(response.length());
            Serial.println(buffer_size);
            Serial.println("Failed to copy response into buffer");
            return false;
        }

        case 401:{
            Serial.printf("Bad Reuqest %d error", status);
        }
        default:

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
    return true;

}

bool HttpLibWrap::downloadCSV(const char* link){
    Serial.println(link);
    const size_t buffer_size = 2048;
    char buffer[buffer_size];
    this->getLink(link, buffer, buffer_size);

    buffer[buffer_size] = '\0';

    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to mount SPIFFS");
        return false;
    }

    //open file
    File file = SPIFFS.open(FILE_NAME, FILE_WRITE);

    if(!file){Serial.println("Failed to open for writing"); return false;}

    //amount of bits written
    size_t written = file.write((uint8_t*)buffer,buffer_size);
    file.close();
    return true;
}
