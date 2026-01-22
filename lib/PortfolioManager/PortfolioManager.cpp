#include <Arduino.h>
#include "IPortfolioManager.h"
#include "PortfolioManager.h"
#include "config.hpp"
#include "PortfolioStats.h"
#include <chrono>
#include <thread>
#include "cJSON.h"
#include "SPIFFS.h"
#include <cstring>

#define FILE_NAME "/accountInfo.csv"
#define ITEM_SIZE 1

using cJson_ptr = std::unique_ptr<cJSON, decltype(&cJSON_Delete)>;

// Implementation of PortfolioManager constructor
PortfolioManager::PortfolioManager(const AccountSubType& type, HttpLibWrap& apiController, CSVReportReader& CSVReader, PortfolioStats& stats)
    : type(type), apiController(apiController), CSVReader(CSVReader), stats(stats){
        //we need to call get of apiController to start and populate values
        apiController.updateAccountSubType(type);

}
/*
// Implementation of update method (if needed)
bool PortfolioManager::update(Stock newStock) {
	// TODO: implement update logic
	return false;
}*/

// Implementation of updateAccountSubType
bool PortfolioManager::updateAccountSubType(const AccountSubType& newType) noexcept {
	type = newType;
    bool result = apiController.updateAccountSubType(newType);
    return result;
}

bool PortfolioManager::getAccountInfo(){

    const size_t buffer_size = 1024;
    static char buffer[buffer_size];
    bool response = apiController.get(Config::T212_ACCOUNT_INFO_ENDPOINT, buffer, buffer_size);
    Serial.println(response);
    if(response){
        Serial.println("Succeeded");
        Serial.println(buffer);
        return true;
    }else{
        Serial.println("Failed to get account info");
        return false;
    }

}

bool PortfolioManager::getAccountHistory(){
    //creates our json object
    const char* time_from = "2025-01-01T00:00:00Z";
    const char* time_to  = "2025-12-06T00:00:00Z";
    cJson_ptr json(cJSON_CreateObject(), cJSON_Delete);

    cJson_ptr dataIncluded(cJSON_CreateObject(), cJSON_Delete);
    cJSON_AddBoolToObject(dataIncluded.get(), "includeDividends", true);
    cJSON_AddBoolToObject(dataIncluded.get(), "includeInterest", true);
    cJSON_AddBoolToObject(dataIncluded.get(), "includeOrders", true);
    cJSON_AddBoolToObject(dataIncluded.get(), "includeTransactions", true);

    cJSON_AddItemToObject(json.get(), "dataIncluded" , dataIncluded.release());
    cJSON_AddStringToObject(json.get(), "timeFrom" , time_from);
    cJSON_AddStringToObject(json.get(), "timeTo" , time_to);

    const size_t buffer_size = 64;
    char response_report_id[buffer_size];
    bool response = apiController.post(Config::T212_REQUEST_CSV_GEN, json, response_report_id, buffer_size);
    
    if(response){
        //get the report via get
        const size_t download_link_buffer_size = 1028;
        //more ;ike all report download links
        char download_link_buffer[download_link_buffer_size];
        Serial.println("Waiting on download link");
        delay(60000);
        bool get_download_link = apiController.get(Config::T212_REQUEST_CSV_GEN, download_link_buffer, download_link_buffer_size, response_report_id);

        if(get_download_link){
            //NOW DOWNLOAD file using the link
            if(apiController.downloadCSV(download_link_buffer)){
                Serial.println("File downloaded successfully");
                return true;

            }else{Serial.println("Error - File not downloaded"); return false;}

        
        }else{Serial.println("Invalid download link"); return false;}
    }
    return true;
}

//refactor to read in and call Stats for each line!
bool PortfolioManager::readInCSV(){
    File file = SPIFFS.open(FILE_NAME, FILE_READ);
    if(!file){return false;}
    while(file.available()){
        const char* line = file.readStringUntil('\n').c_str();
        Serial.println(line);
    }
    file.close();
    return true;
}