#ifndef CSVREPORTREADER_H
#define CSVREPORTREADER_H

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
        std::cout << "Deposit - Time: " << time.print() << ", ID: " << id 
        << ", Total: " << total 
        << " " << currency << std::endl;
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
        std::cout << "Market Buy - Time: " << time.print()
        << ", ID: " << id
        << ", Ticker: " << ticker
        << ", Shares: " << NoShares
        << ", Price: " << pricePerShare << " Currency for price per share: " << currencyPricePerShare
        << ", Total: " << total << " Currency: " 
        << currency << std::endl;
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

        std::vector<Action>& readInFile();
        void setFilePath(const std::string& filePath) noexcept;
        void createAction(const std::string& line);
        void valueActionIterator(const std::vector<std::string>& values);
};
#endif