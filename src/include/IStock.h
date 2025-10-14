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

    public:
        virtual ~IStock();

        //not sure about setters???

        virtual float getAvgPrice() const = 0;
        virtual void setAvgPrive(float newAvgPrice) = 0;

        virtual float getCurrPrice() const = 0;
        virtual void setCurrPrice(float newCurrPrice) = 0;

        virtual std::string getStockInfo() const = 0;
        virtual void setStockInfo(std::string newStockInfo) = 0;

        virtual float getQuantity() const = 0;
        virtual void setQuantity(float newQuantity) = 0;

        virtual std::string getUniqueID() const = 0;
        virtual void setUniqueID(std::string newUniqueID) = 0;
};

#endif
