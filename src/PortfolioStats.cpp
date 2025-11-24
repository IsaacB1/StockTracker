#include "IPortfolioStats.h"
#include "PortfolioStats.h"
#include <string> 
#include <iostream>
#include "CSVReportReader.h"

void PortfolioStats::updateStats(const double& total_invested, const double& curr_value, const double& total_free) noexcept{
    this->set_total_invested(total_invested);
    this->set_curr_value(curr_value);
    this->set_total_made( curr_value - total_invested);
    this->set_percent_change(total_invested);
    this->set_total_free(total_free);
}

void PortfolioStats::printStats() noexcept{
    std::cout << "You have £" << this->total_free << " deposited \nYou have invested: \n" << this->total_invested << "\n" << "Current value: \n" << this->curr_value << "\n" << "Money made: \n" << this->total_made << std::endl;


    std::cout << "You hold: " << std::endl; 
    for(const auto& [ticker, stock] : this->stocks){
        std::cout << stock.name << " (" << stock.ISIN << "): " << stock.noOfShares << " shares" << std::endl;
    }
}

void PortfolioStats::addStock(const Stock& newStock, const std::string& ticker){
    auto it = this->stocks.find(ticker);
    //in map
    if(it != this->stocks.end()){
        it->second.noOfShares += newStock.noOfShares;
    //not in map
    } else {
        this->stocks.emplace(ticker, newStock);
    }
}

void PortfolioStats::minusStock(const Stock& oldStock, const std::string& ticker){
    auto it = this->stocks.find(ticker);
    //in map
    if(it != this->stocks.end()){
        if(it->second.noOfShares -= oldStock.noOfShares < 0) throw std::out_of_range("cant sell more then you have");
    //not in map
    } else{ throw std::invalid_argument("No stock found ");}
}

void PortfolioStats::syncStocks(const std::vector<Action>& actions){
    for(const Action& action : actions){

        if(std::holds_alternative<MarketBuy>(action)){
            const MarketBuy& mb = std::get<MarketBuy>(action);
            Stock newStock;
            newStock.name = mb.Name;
            newStock.ISIN = mb.ISIN;
            newStock.noOfShares = mb.NoShares;

            this->addStock(newStock, mb.ticker);
        /*Not implemented yet
        }else if(std::holds_alternative<MarketSell>(action)){*/

        //do something here I guess 
        }else if(std::holds_alternative<Deposit>(action)){

        }
    }
}