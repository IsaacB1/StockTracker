#include <Arduino.h>
#include <iostream>
#include "HttpLibWrap.h"
#include <string>
#include "config.hpp"
#include <string_view>
#include "AccountSubType.h"
#include "PortfolioManager.h"
#include "CSVReportReader.h"
#include "PortfolioStats.h"

int run() {
    const AccountSubType accountSubType = AccountSubType::StocksISA;

    
    HttpLibWrap stocksISAAPI = HttpLibWrap();

    CSVReportReader CSVReader = CSVReportReader();
    PortfolioStats stats = PortfolioStats();

    PortfolioManager StocksISAManager = PortfolioManager(accountSubType, stocksISAAPI, CSVReader, stats);

    StocksISAManager.getAccountInfo();

    std::cout << "getting accout history" << std::endl;
        StocksISAManager.getAccountHistory();


    return 0;
}

//would be cool to implement multithreding here for API calls
void setup() {
    Serial.begin(115200);
    // initialize your code
    run();
}
void loop(){}
