#include "IPortfolioManager.h"
#include "PortfolioManager.h"
#include <dotenv.h>


// Implementation of PortfolioManager constructor
PortfolioManager::PortfolioManager(const AccountSubType& type, HttpLibWrap& apiController)
	: type(type), apiController(apiController){
        //we need to call get of apiController to start and populate values
        std::cout << "Got here" << std::endl;
        dotenv::init();

        try{
            APIResponse response;
            response = apiController.get(std::getenv("T212_ENDPOINT"));

            if(response.status == 200){
                //then we add parsin logic here so populate Portfolio Manager
                std::cout << "Call successfull" << std::endl;
            }

        }catch(const std::runtime_error e){
            std::cerr << "Caught exception: " << e.what() << std::endl;
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
