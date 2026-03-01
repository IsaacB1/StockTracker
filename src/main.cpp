#include <Arduino.h>
#include "HttpLibWrap.h"
#include <string>
#include "config.hpp"
#include <string_view>
#include "AccountSubType.h"
#include "PortfolioManager.h"
#include "CSVReportReader.h"
#include "PortfolioStats.h"
#include "UserInter.h"
#include <TFT_eSPI.h>
#include <memory>


TFT_eSPI tft = TFT_eSPI();

int run(PortfolioManager &StocksISAManager, UserInter &ui) {
    std::unique_ptr<TFT_eSprite> loading_sprite = ui.startLoading();
    StocksISAManager.getAccountInfo();
    StocksISAManager.getAccountHistory();
    ui.endLoading(std::move(loading_sprite));

    StocksISAManager.readInCSV();

    return 0;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting");

    const AccountSubType accountSubType = AccountSubType::StocksISA;
    UserInter ui = UserInter();
    HttpLibWrap stocksISAAPI = HttpLibWrap();
    stocksISAAPI.wifiSetup();
    CSVReportReader CSVReader = CSVReportReader();
    PortfolioStats stats = PortfolioStats();

    PortfolioManager StocksISAManager = PortfolioManager(accountSubType, stocksISAAPI, CSVReader, stats);


    //set up screen

    
    run(StocksISAManager, ui);
}

void loop(){}

