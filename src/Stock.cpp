#include "Stock.h"
#include <string> 

class Stock : public IStock {

    private:
        float avgPrice;
        float currPrice;
        std::string stockInfo;
        float quantity;
        std::string uniqueID;

    public:
        Stock(float avgPrice, float currPrice, std::string stockInfo, float quantity, std::string uniqueID)
            : avgPrice(avgPrice), currPrice(currPrice), stockInfo(stockInfo), quantity(quantity), uniqueID(uniqueID) {};

        float getAvgPrice() const { return this->avgPrice; }
        void setAvgPrice( float newAvgPrice ) {avgPrice = newAvgPrice;}

        float getCurrPrice() const { return this->currPrice; }
        void setCurrPrice( float newCurrPrice ) {currPrice = newCurrPrice;}

        std::string getStockInfo() const { return this->stockInfo; }
        void setStockInfo( float newStockInfo ) {stockInfo = newStockInfo;}

        float quantity() const { return this->quantity; }
        void setQuantity( float newQuantity ) {quantity = newQuantity;}

        std::string getUniqueID() const { return this->uniqueID; }
        void setUniquID( float newUniqueID) {uniqueID = newUniqueID;}
};