#include "IPortfolioManager.h"
#include "PortfolioManager.h"
#include <dotenv.h>
#include <json.hpp>

// Implementation of PortfolioManager constructor
PortfolioManager::PortfolioManager(const AccountSubType& type, HttpLibWrap& apiController)
	: type(type), apiController(apiController){
        //we need to call get of apiController to start and populate values
        dotenv::init();
        using json = nlohmann::json;

        apiController.updateAccountSubType(type);

        try{
            APIResponse response;
            response = apiController.get(std::getenv("T212_ENDPOINT"));

            if(response.status == 200){
                //then we add parsin logic here so populate Portfolio Manager
                //using the AccountVakue type

                //probably add this to a function rather then jsut ehre
                AccountValue callVal;
                std::cout << response.body <<std::endl;
                //Using json.hpp to parse the json 
                json apiResponse = json::parse(response.body);

                callVal.invested = apiResponse["invested"];
                callVal.value = apiResponse["total"];
                callVal.totalMade = (callVal.value - callVal.invested);
                
                std::cout << "callVals - invested: " << callVal.invested << " value: "<< callVal.value << " total made: " << callVal.totalMade << std::endl;
            }

        }catch(const std::runtime_error e){
            std::cerr << "Failed to get .env value - endpoint: " << e.what() << std::endl;
        }
}

// Implementation of update method (if needed)
bool PortfolioManager::update(Stock newStock) {
	// TODO: implement update logic
	return false;
}

// Implementation of updateAccountSubType
void PortfolioManager::updateAccountSubType(const AccountSubType& newType) noexcept {
	type = newType;
}
