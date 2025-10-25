#include <string>
#include "IStock.h"
#ifndef STOCK_H
#define STOCK_H

class Stock : public IStock{

    private:
        std::string dateCreated;
        std::string dateExecuted;
        std::string dateModified;

        //how many shares
        double filledQuantity;
        //price of share
        double filledPrice;
        std::string stockName;

    public:
        Stock(const std::string& dateCreated,const std::string& dateExecuted,const std::string& dateModified,const double& filledQuantity,const double& filledPrice,const std::string& stockName);

        //getters
        std::string getDateExecuted() noexcept;
        std::string getDateCreated() noexcept;
        std::string getDateModified() noexcept;
        std::string getStockName() noexcept;
        double getFilledQuantity() noexcept;
        double getFilledPrice() noexcept;

        ~Stock() = default;
};

#endif