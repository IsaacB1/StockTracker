#ifndef ACCOUNT_HISTORYUI_H
#define ACCOUNT_HISTORYUI_H
#include "UserInter.h"
#include "CSVReportReader.h"

class AccountHistoryUI {
public:
    AccountHistoryUI();
    bool displayHistory(const std::vector<Action>&, UserInter &ui);
};
#endif