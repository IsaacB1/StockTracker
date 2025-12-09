#include <Arduino.h>
#include "IPortfolioManager.h"
#include "PortfolioManager.h"
#include "config.hpp"
#include "PortfolioStats.h"
#include <chrono>
#include <thread>
#include "cJSON.h"
#include "SPIFFS.h"

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

    char response_buffer_link[1024];
    bool response = apiController.post(Config::T212_REQUEST_CSV_GEN, json, response_buffer_link);
    
    if(response){
        bool download_response = apiController.downloadCSV(response_buffer_link);

        if(download_response){
            //at this point its been saved as a file
            //reads in file line by line
            bool readInCSV = this->readInCSV();

            if(!readInCSV){return false;}
        }else{Serial.println("Failed to downlaod file"); return false;}
    }else{ Serial.println("Call failed -> or didnt even work"); return false;}
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