#include <iostream>
#include "HttpLibWrap.h"
#include <string>
#include <dotenv.h>
#include <string_view>
#include "AccountSubType.h"
#include "PortfolioManager.h"


int main() {
    dotenv::init();
    const AccountSubType accountSubType = AccountSubType::StocksISA;

    //create APIWrapper class
    try{
        HttpLibWrap stocksISAAPI = HttpLibWrap(std::getenv("API_HOST"));

        PortfolioManager StocksISAManager = PortfolioManager(accountSubType, stocksISAAPI);

        StocksISAManager.getAccountInfo();

        std::cout << "getting accout history" << std::endl;
        StocksISAManager.getAccountHistory();

    }catch(const std::runtime_error& e){
        std::cerr << "Failed to get .env value - API host: " << e.what() << std::endl;
        return 1;
    }


    return 0;
}