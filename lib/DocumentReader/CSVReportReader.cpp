#include "CSVReportReader.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "SPIFFS.h"

#define FILE_NAME "/accountInfo.csv"

CSVReportReader::CSVReportReader() : filePath(), actions(std::vector<Action>()){
}

std::vector<Action>& CSVReportReader::getActions(){
    return this->actions;
}

//feels like this needs to be propelry returning errors rahter then booleans
bool CSVReportReader::readInFile(){

    File file = SPIFFS.open(FILE_NAME, FILE_READ);
    if(!file){Serial.println("No file available"); return false;}
    bool first = true;
    while(file.available()){
        String lineStr = file.readStringUntil('\n');
        const char* line = lineStr.c_str();
        //Serial.println(line);
        // don't spam the serial monitor with the header/first line
        if (first) {
            first = !first;
        }else{
        // parse each line; stop on first failure
            if (!this->createAction(line)) {
                Serial.println("Failed to parse CSV line");
                return false;
            }
        }
    }
    file.close();
    return true;
}

bool CSVReportReader::createAction(const std::string& line){

    std::stringstream ss(line);
    std::vector<std::string> values;
    //temporary 
    std::string field;

    //this stores each value in one row into values-vector
    while(std::getline(ss, field, ',')){
        values.push_back(field);
    }

    if (values.empty()) {
        Serial.println("Action to create is empty");
        return false;
    }
    std::string type = values.front();
    //Serial.println(type.c_str());
    //Serial.println(values.size());
    if (type == "Deposit") {
        // require at least 13 fields
        if (values.size() > 12){Serial.println("Line not correct length"); return false;}
        Deposit dep;
        Time depTime;
        depTime.date = values[1];
        dep.id = values[5];
        try {
            dep.total = std::stod(values[10]);
        } catch (...) {
            Serial.println("Failed to convert total string to int");
            return false;
        }
        dep.currency = values[11];
        dep.time = depTime;
        this->actions.push_back(dep);
        return true;
    } else if (type == "Market buy") {
        if (values.size() > 12){ return false;}
        MarketBuy markBuy;
        Time markBuyTime;
        markBuyTime.date = values[1];
        markBuy.ISIN = values[2];
        markBuy.ticker = values[3];
        markBuy.Name = values[4];
        markBuy.id = values[5];
        try {
            markBuy.NoShares = std::stod(values[6]);
            markBuy.pricePerShare = std::stod(values[7]);
            markBuy.exchangeRate = std::stod(values[9]);
            markBuy.total = std::stod(values[10]);
        } catch (...) {
            Serial.println("Failed to convert total string to int");
            return false;
        }
        markBuy.currencyPricePerShare = values[8];
        markBuy.currency = values[11];
        markBuy.time = markBuyTime;
        this->actions.push_back(markBuy);
        return true;
    } else {
        Serial.println("Tryed to create unrecognised action");
        return false;
    }
}

void CSVReportReader::setFilePath(const std::string& filePath) noexcept{
    this->filePath = filePath;
}

bool CSVReportReader::readOutActions(){
    // `Action` is a std::variant<Deposit,MarketBuy> so we can't call
    // `print()` on it directly.  Use std::visit to dispatch to the
    // underlying type's member function.
    for (Action &action : this->actions) {
        std::visit([](auto &a) { a.print(); }, action);
    }
    return true;
}
