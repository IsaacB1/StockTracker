#ifndef CSVREPORTREADER_H
#define CSVREPORTREADER_H

#include <iostream>
#include <string>
#include <vector>
#include "IDocumentReader.h"

//specifc time struct
struct Time{
    std::string date;
    std::string specificTime;

};

// These are structs for differnet actions that have happened

struct Deposit{
    Time depositTime;
    std::string id;
    double total;
    std::string currency;
};

struct MarketBuy{
    Time depositTime;
    std::string ISIN;
    std::string ticker;
    std::string Name;
    std::string ID;
    double NoShares;
    double pricePerShare;
    std::string currencyPricePerShare;
    double exchangeRate;
    double total;
    std::string currency;
};

//Not implemented need to do more research on the fiekds
struct MarketSell{

};

struct Sell{

};

struct Withdraw{

};

struct Dividend{

};

struct ActionType{
    Deposit deposit;
    MarketBuy marketBuy;
    MarketSell marketSell;
    Sell sell;
    Withdraw withdraw;
    Dividend div;

    ActionType() {}
    ~ActionType() {}
    ActionType(const ActionType&) = default;
    ActionType& operator=(const ActionType&) = default;
};

//NEED TO REFORM STRUCTS

//currently class only designed to read in CSV ONCE, no update functionality
class CSVReportReader : public IDocumentReader {

    private:
        std::string filePath;
        std::vector<ActionType> actions;

    public:
        CSVReportReader();
        ~CSVReportReader() = default;

        bool readInFile();
        void setFilePath(const std::string& filePath) noexcept;
        std::vector<std::string> splitField(const std::string& line);
};
#endif