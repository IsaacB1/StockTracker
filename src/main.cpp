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
    std::cout << "HDHDHDH" << std::endl;
    HttpLibWrap stocksISAAPI = HttpLibWrap(std::getenv("API_HOST"), accountSubType);

    PortfolioManager StocksISAManager = PortfolioManager(accountSubType, stocksISAAPI);

    return 0;
}