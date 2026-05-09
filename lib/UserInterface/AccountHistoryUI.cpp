#include "UserInter.h"
#include "AccountHistoryUI.h"
#include "DisplayGuard.h"
#include <iostream>
#include <vector>
#include "CSVReportReader.h"

AccountHistoryUI::AccountHistoryUI(){}

bool AccountHistoryUI::displayHistory(const std::vector<Action>& actions, UserInter &ui){
    constexpr int text_x = 5;
    constexpr int text_y = 20;
    constexpr int text_scale = 1;
    constexpr int display_delay = 5;
    Serial.println("here");
    for(const Action &action : actions){
        Serial.println("Printing history");
        std::visit([&ui, text_x, text_y, text_scale, display_delay](const auto &a) {
            Serial.println("Visited");
            DisplayGuard guard(ui, a.toString().c_str(), TFT_RED,
            text_scale, text_x, text_y, display_delay);
        }, action);
        delay(2000);
    }

    Serial.println("Finished printing actions");
    return true;
}