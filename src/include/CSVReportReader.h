#include <iostream>
#include <string>
#include <vector>
#include "IDocumentReader.h"

#ifndef CSVREPORTREADER_H
#define CSVREPORTREADER_H

struct Deposit{

};

struct MarketBuy{

};

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
};

class CSVReportReader : IDocumentReader {

    private:
        std::string filePath;
        std::vector<ActionType> actions;
};
#endif