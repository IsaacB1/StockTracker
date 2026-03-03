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

int run(PortfolioManager &StocksISAManager, UserInter &ui) {
    const char* account_info_text = "Making Get call ... Account Info";
    const int account_info_x = 5;
    const int account_info_y = 20;
    const int account_info_scale = 1;
    const int local_account_info_delay = 5;
    std::unique_ptr<TFT_eSprite> getAccountInfoText = ui.displayText(account_info_text, TFT_RED, account_info_scale, account_info_x, account_info_y,local_account_info_delay);
    StocksISAManager.getAccountInfo();
    ui.endDisplayText(std::move(getAccountInfoText), account_info_x, account_info_y);

    StocksISAManager.getAccountHistory();


    StocksISAManager.readInCSV();

    return 0;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting");
    UserInter ui = UserInter();
    std::unique_ptr<TFT_eSprite> loading_sprite = ui.startLoading();

    const AccountSubType accountSubType = AccountSubType::StocksISA;

    HttpLibWrap stocksISAAPI = HttpLibWrap();
    CSVReportReader CSVReader = CSVReportReader();
    PortfolioStats stats = PortfolioStats();

    PortfolioManager StocksISAManager = PortfolioManager(accountSubType, stocksISAAPI, CSVReader, stats);

    ui.endLoading(std::move(loading_sprite));
    run(StocksISAManager, ui);
}

void loop(){}

