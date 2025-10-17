#include <iostream>
#include <string>
#include <vector>
#include "Stock.h"
#include "AccountSubType.h"

#ifndef IPORTFOLIOMANAGER_H
#define IPORTFOLIOMANAGER_H

struct AccountValue{
    int invested;
    int value;
    int totalMade;
};


class IPortfolioManager {
    public:
        //need to pass some input from APi to fill stocks vector
        //need to pass aome input here as well
        //bool IPortfolioManager::update(Stock newStock);
        void updateAccountSubType(const AccountSubType& newType) noexcept;
        ~IPortfolioManager() = default;
}; 

#endif