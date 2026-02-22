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

TFT_eSPI tft = TFT_eSPI();

int run( PortfolioManager StocksISAManager) {
    StocksISAManager.getAccountInfo();
    StocksISAManager.getAccountHistory();

    return 0;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting");
    const AccountSubType accountSubType = AccountSubType::StocksISA;

    HttpLibWrap stocksISAAPI = HttpLibWrap();
    stocksISAAPI.wifiSetup();
    CSVReportReader CSVReader = CSVReportReader();
    PortfolioStats stats = PortfolioStats();

    PortfolioManager StocksISAManager = PortfolioManager(accountSubType, stocksISAAPI, CSVReader, stats);


    //set up screen

    UserInter interface = UserInter();


    run(StocksISAManager);
}

void loop(){}

/*
//would be cool to implement multithreding here for API calls
void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Booting");
    // initialize your code
    pinMode(22, OUTPUT); // DC
    pinMode(4, OUTPUT);  // RST

    digitalWrite(4, LOW);   // reset low
    delay(100);
    digitalWrite(4, HIGH);  // reset high
    delay(100);

    tft.begin();
    tft.fillScreen(TFT_WHITE);
    run();
}
void loop(){
    tft.fillScreen(TFT_RED);
    delay(1000);
    tft.fillScreen(TFT_BLUE);
    delay(1000);
}*/
