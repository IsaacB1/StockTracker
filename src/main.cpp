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

// RAII wrapper to handle automatic display text cleanup
class DisplayGuard {
private:
    UserInter& ui;
    std::unique_ptr<TFT_eSprite> sprite;
    int x, y;
public:
    DisplayGuard(UserInter& ui, const char* text, uint16_t colour, int scale, int x, int y, int delay)
        : ui(ui), x(x), y(y), sprite(ui.displayText(text, colour, scale, x, y, delay)) {}
    ~DisplayGuard() { ui.endDisplayText(std::move(sprite), x, y); }
};

int run(PortfolioManager &StocksISAManager, UserInter &ui) {
    constexpr int text_x = 5;
    constexpr int text_y = 20;
    constexpr int text_scale = 1;
    constexpr int display_delay = 5;

    // {} create the scope for cleanup
    {
        DisplayGuard guard(ui, "Making Get call ... Account Info", TFT_RED,
            text_scale, text_x, text_y, display_delay);
        StocksISAManager.getAccountInfo();
    }

    {
        DisplayGuard guard(ui, "Making Post call ... Account History", TFT_RED,
            text_scale, text_x, text_y, display_delay);
        StocksISAManager.getAccountHistory();
    }

    {
        DisplayGuard guard(ui, "Saving History to file ...", TFT_RED,
            text_scale, text_x, text_y, display_delay);
        StocksISAManager.readInCSV();
        delay(2000);
    }

    return 0;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting");
    UserInter ui = UserInter();
    std::unique_ptr<TFT_eSprite> loading_sprite = ui.startLoading();

    const AccountSubType accountSubType = AccountSubType::Stocks;

    HttpLibWrap stocksISAAPI = HttpLibWrap();
    CSVReportReader CSVReader = CSVReportReader();
    PortfolioStats stats = PortfolioStats();

    PortfolioManager StocksISAManager = PortfolioManager(accountSubType, stocksISAAPI, CSVReader, stats);

    ui.endLoading(std::move(loading_sprite));
    run(StocksISAManager, ui);
}

void loop(){
}

