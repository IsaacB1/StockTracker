#ifndef CSVREPORTREADER_H
#define CSVREPORTREADER_H

#include <Arduino.h>
#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include "IDocumentReader.h"

//specifc time struct
struct Time {
    std::string date;
    std::string specificTime;
    std::string print() const {
        return  "Time at " + date + " " + specificTime;
    };
};

/*
enum class Action {
    Action() = default;
    virtual ~Action() = default;
    virtual void print() const = 0;
    Action(const Action&) = default;
    Action(Action&&) = default;
    Time time;
    std::string id;
};*/

// These are structs for differnet actions that have happened
struct Deposit{
    double total;
    std::string id;
    std::string currency;
    Time time;
    void print() const {
        String out = "Deposit - Time: ";
        out += time.print().c_str();
        out += ", ID: ";
        out += id.c_str();
        out += ", Total: ";
        out += total;                      
        out += " ";
        out += currency.c_str();
        Serial.println(out);
    }
};

struct MarketBuy {
    std::string id;
    Time time;
    std::string ISIN;
    std::string ticker;
    std::string Name;
    double NoShares;
    double pricePerShare;
    std::string currencyPricePerShare;
    double exchangeRate;
    double total;
    std::string currency;

    void print() const {
        String out = "Market Buy - Time: ";
        out += time.print().c_str();
        out += ", ID: ";
        out += id.c_str();
        out += ", Ticker: ";
        out += ticker.c_str();
        out += ", Shares: ";
        out += NoShares;
        out += ", Price: ";
        out += pricePerShare;
        out += " Currency for price per share: ";
        out += currencyPricePerShare.c_str();
        out += ", Total: ";
        out += total;
        out += " Currency: ";
        out += currency.c_str();
        Serial.println(out);
    }
};

//this creates type Action with all the subtypes valid
using Action = std::variant<Deposit, MarketBuy>;

//NEED TO REFORM STRUCTS

//currently class only designed to read in CSV ONCE, no update functionality
class CSVReportReader : public IDocumentReader {

    private:
        std::string filePath;
        std::vector<Action> actions;

    public:
        CSVReportReader();
        ~CSVReportReader() = default;

        bool readInFile();
        void setFilePath(const std::string& filePath) noexcept;
        bool createAction(const std::string& line);
        bool readOutActions();
        std::vector<Action>& getActions();
};
#endif