#include <string>
#include "IStock.h"
#ifndef STOCK_H
#define STOCK_H

class Stock : public IStock{

    private:
        float avgPrice;
        float currPrice;
        std::string stockInfo; //stuff like when bought
        float quantity;
        std::string uniqueID;

    public:

        Stock(float avgPrice, float currPrice, std::string stockInfo, float quantity, std::string uniqueID);

        virtual float getAvgPrice() const override;
        virtual void setAvgPrive(float newAvgPrice) override;

        virtual float getCurrPrice() const override;
        virtual void setCurrPrice(float newCurrPrice) override;

        virtual std::string getStockInfo() const override;
        virtual void setStockInfo(std::string newStockInfo) override;

        virtual float getQuantity() const override;
        virtual void setQuantity(float newQuantity) override;

        virtual std::string getUniqueID() const override;
        virtual void setUniqueID(std::string newUniqueID) override;
};

#endif