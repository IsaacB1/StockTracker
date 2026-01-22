#include <Arduino.h>
#include "HttpLibWrap.h"
#include <string>
#include "config.hpp"
#include <string_view>
#include "AccountSubType.h"
#include "PortfolioManager.h"
#include "CSVReportReader.h"
#include "PortfolioStats.h"

int run() {
    Serial.println("Starting");
    const AccountSubType accountSubType = AccountSubType::StocksISA;

    HttpLibWrap stocksISAAPI = HttpLibWrap();
    stocksISAAPI.wifiSetup();
    CSVReportReader CSVReader = CSVReportReader();
    PortfolioStats stats = PortfolioStats();

    PortfolioManager StocksISAManager = PortfolioManager(accountSubType, stocksISAAPI, CSVReader, stats);

    StocksISAManager.getAccountInfo();
    StocksISAManager.getAccountHistory();

    return 0;
}

//would be cool to implement multithreding here for API calls
void setup() {
    Serial.begin(115200);
    // initialize your code
    delay(1000);
    run();
}
void loop(){
}
