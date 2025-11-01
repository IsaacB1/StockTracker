#include "IPortfolioManager.h"
#include "PortfolioManager.h"
#include <dotenv.h>
#include <json.hpp>
#include "Stock.h"
#include <chrono>
#include <thread>

// Implementation of PortfolioManager constructor
PortfolioManager::PortfolioManager(const AccountSubType& type, HttpLibWrap& apiController, CSVReportReader& CSVReader)
	: type(type), apiController(apiController), CSVReader(CSVReader){
        //we need to call get of apiController to start and populate values
        apiController.updateAccountSubType(type);

}
/*
// Implementation of update method (if needed)
bool PortfolioManager::update(Stock newStock) {
	// TODO: implement update logic
	return false;
}*/

// Implementation of updateAccountSubType
void PortfolioManager::updateAccountSubType(const AccountSubType& newType) noexcept {
	type = newType;
}

bool PortfolioManager::getAccountInfo(){
    dotenv::init();
    using json = nlohmann::json;

    try{
        APIResponse response;
        response = apiController.get(std::getenv("T212_ACCOUNT_INFO_ENDPOINT"));

        if(response.status == 200){

            AccountValue callVal;
            std::cout << response.body <<std::endl;
            //Using json.hpp to parse the json 
            try{
                json apiResponse = json::parse(response.body);

                callVal.invested = apiResponse["invested"];
                callVal.value = apiResponse["total"];
                callVal.totalMade = (callVal.value - callVal.invested);
                    
                std::cout << "callVals - invested: " << callVal.invested << " value: "<< callVal.value << " total made: " << callVal.totalMade << std::endl;
                return true;
            }catch(const json::parse_error& e){
                std::cerr << "Failed to parse accountvalue api response: " << e.what() << std::endl;

                return false;
            }
        }else{
            std::cerr << "Response code " << response.status << "recieved" << std::endl;
            return false;
        }

    }catch(const std::runtime_error& e){
        std::cerr << "Failed to get .env value - endpoint: " << e.what() << std::endl;
        return false;
    }
}

bool PortfolioManager::getAccountHistory(){
    //will return APIRepsonse type with reportId as body and status as status

    //gets present day  NEEDS TO BE IN OWN FUNCTION 
    auto now = std::chrono::system_clock::now() - std::chrono::minutes(1);
    std::time_t t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_utc;
    gmtime_r(&t_now, &tm_utc);

    std::ostringstream oss;
    oss << std::put_time(&tm_utc, "%Y-%m-%dT%H:%M:%SZ");

    //create body to get parsed 
    nlohmann::json body = { {"dataIncluded", {{"includeDividends", true},{"includeInterest", true},{"includeOrders", true},{"includeTransactions", true}}},{"timeFrom", "2025-01-01T00:00:00Z"},{"timeTo", oss.str()}
    };

    std::cout << "making post call" << std::endl;
    try{
        APIResponse requestIdResponse = apiController.post(std::getenv("T212_REQUEST_CSV_GEN"), body);
        
        try{
            std::cout << requestIdResponse.body << std::endl;
            json requestIdApiResponse = json::parse(requestIdResponse.body);

            std::cout << requestIdApiResponse << std::endl;

            //need a try here YES I DO
            int requestId = requestIdApiResponse["reportId"].get<int>();
            
            //also try here for the std::getenv
            //now do the get request 
            std::string endpoint = std::getenv("T212_REQUEST_CSV_GEN");
            //endpoint = endpoint + "/" + std::to_string(requestId);
            std::cout << endpoint << std::endl;

            //chill for 20 seconds as it takes a sec to get the report ready
            std::this_thread::sleep_for(std::chrono::milliseconds(20000));

            //sort out here may need to do a time out as 404 means it hasnt generated the page yet
            APIResponse historyResponse;
            try{
                int maxRetries = 30;
                //1 Minute as thats what the API limits this to
                int retryDelayMs = 60000;

                for (int i = 0; i < maxRetries; ++i) {
                    historyResponse = apiController.get(endpoint);

                    if (historyResponse.status == 200) {
                        std::cout << "Export ready, processing..." << std::endl;
                        json historyApiResponse = json::parse(historyResponse.body);
                        const json latestReport = historyApiResponse.back();
                        if (latestReport["status"] != "Finished"){
                            std::cout << "Report Not ready yet waiting 20 seconds minute" << std::endl;
                            std::this_thread::sleep_for(std::chrono::milliseconds(retryDelayMs));
                        }else{
                            break;
                        }
                    } else if (historyResponse.status == 404) {
                        std::cout << "Export not ready yet (status 404), retrying..." << std::endl;
                        std::cout << "Potiential body " << historyResponse.body << std::endl;
                        std::this_thread::sleep_for(std::chrono::milliseconds(retryDelayMs));
                    } else if (historyResponse.status == 429){
                        throw std::runtime_error("Too many requests");
                    } else {
                        std::cerr << "Unexpected status: " << historyResponse.status << std::endl;
                        break;
                    }
                }

            }catch(const std::runtime_error& e){
                std::cerr << "Caught runtime error: " << e.what() << std::endl;
            }
            

            //now sort out json response
            try{

                //std::cout << historyResponse.body << std::endl;

                json historyApiResponse = json::parse(historyResponse.body);

                //std::cout << historyApiResponse << std::endl;

                //for this we need to parse based off 
                /* "dataIncluded": {
                    "includeDividends": true,
                    "includeInterest": true,
                    "includeOrders": true,
                    "includeTransactions": true
                }, */
                const json latestReport = historyApiResponse.back();
                std::cout << latestReport << std::endl;

                std::string link = latestReport["downloadLink"];
                std::string status = latestReport["status"];
                std::string timeFrom = latestReport["timeFrom"];
                std::string timeTo = latestReport["timeTo"];

                std::cout << link << std::endl;

                try{
                    std::string filePath = apiController.downloadCSV(link);

                    if (this->readInCSV(filePath)){

                    }else{

                    }
                }catch(const std::runtime_error& e){
                    std::cerr << e.what() << std::endl;
                }
            }catch(const json::parse_error& e){
                std::cerr << "JSON parse error when trying to parse historyAPI response " << e.what() << std::endl;
            }
        }catch(const json::parse_error& e){
            std::cerr << "Failed to get requestId from POST call " << e.what() << std::endl;
        }
    }catch(const std::runtime_error& e){
        std::cerr << "Failed to get .env value: T212_REQUEST_CSV_GEN " << e.what() << std::endl;
    }
    return false;
}

bool PortfolioManager::readInCSV(const std::string& filePath){
    this->CSVReader.setFilePath(filePath);

    if(this->CSVReader.readInFile()){
        return true;
    }else{
        return false;
    }
}