/* This is the average return from Trading 212 API 
[
    {
        "averagePrice": 0,
        "currentPrice": 0,
        "frontend": "API",
        "fxPpl": 0,
        "initialFillDate": "2019-08-24T14:15:22Z",
        "maxBuy": 0,
        "maxSell": 0,
        "pieQuantity": 0,
        "ppl": 0,
        "quantity": 0,
        "ticker": "AAPL_US_EQ"
    }
]*/
#include <string>
#ifndef ISTOCK_H
#define ISTOCK_H

class IStock{
    private:
        float avgPrice;
        float currPrice;
        std::string stockInfo; //stuff like when bought
        float quantity;
        std::string uniqueID;

    public:
        IStock(float avgPrice, float currPrice, std::string stockInfo, float quantity, std::string uniqueID);
        ~IStock();

        //not sure about setters???

        float getAvgPrice();
        void setAvgPrive(float newAvgPrice);

        float getCurrPrice();
        void setCurrPrice(float newCurrPrice);

        std::string getStockInfo();
        void setStockInfo(std::string newStockInfo);

        float getQuantity();
        void setQuantity(float newQuantity);

        std::string getUniqueID();
        void setUniqueID(std::string newUniqueID);
};

#endif