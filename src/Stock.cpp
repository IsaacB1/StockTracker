#include "Stock.h"
#include "IStock.h"
#include <string> 

Stock::Stock(const std::string& dateCreated,const std::string& dateExecuted,const std::string& dateModified,const double& filledQuantity,const double& filledPrice,const std::string& stockName) : dateCreated(dateCreated), dateExecuted(dateExecuted),dateModified(dateModified), filledQuantity(filledQuantity), filledPrice(filledPrice),stockName(stockName) {}


std::string Stock::getDateCreated() noexcept {return this->dateCreated;}
std::string Stock::getDateExecuted() noexcept{return this->dateExecuted;}
std::string Stock::getDateModified() noexcept{return this->dateModified;}
std::string Stock::getStockName() noexcept{return this->stockName;}
double Stock::getFilledQuantity() noexcept{return this->filledQuantity;}
double Stock::getFilledPrice() noexcept 
{return this->filledPrice; }
