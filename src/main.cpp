#include <iostream>
#include "HttpLibWrap.h"
#include <string>
#include "config.hpp"
#include <string_view>
#include "AccountSubType.h"
#include "PortfolioManager.h"
#include "CSVReportReader.h"
#include "PortfolioStats.h"


//would be cool to implement multithreding here for API calls

int main() {
    dotenv::init();
    const AccountSubType accountSubType = AccountSubType::StocksISA;

    //create APIWrapper class
    try{
        HttpLibWrap stocksISAAPI = HttpLibWrap(Config::API_HOST);

        CSVReportReader CSVReader = CSVReportReader();
        PortfolioStats stats = PortfolioStats();

        PortfolioManager StocksISAManager = PortfolioManager(accountSubType, stocksISAAPI, CSVReader, stats);

        StocksISAManager.getAccountInfo();

        std::cout << "getting accout history" << std::endl;
        StocksISAManager.getAccountHistory();

        //create document reader and read in data
        //NEED TO CHANGE PORTFOLIO MANAGER TO ACCEPT INSTANCE OF PORTFOLIO MANAGER!!!!

    }catch(const std::runtime_error& e){
        std::cerr << "Failed to get .env value - API host: " << e.what() << std::endl;
        return 1;
    }


    return 0;
}