#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "IAPIClient.h"
#include "base64.h"
#include <cstdlib>
#include "HttpLibWrap.h"
#include <string>
#include <string_view>
#include <sstream>
#include "AccountSubType.h"
#include <dotenv.h>
#include <fstream>
#include <json.hpp>
#include <filesystem>

using json = nlohmann::json;

//need to add API secrets now
HttpLibWrap::HttpLibWrap(const std::string& host) : cli(host.c_str()){
    cli.enable_server_certificate_verification(false);

}   
//nmeed to implement requests now
APIResponse HttpLibWrap::get(const   std::string_view& endpoint) {
    auto request = cli.Get(std::string(endpoint));

    //if it failed
    if(!request){
        auto err = request.error(); 
        std::ostringstream oss;
        oss << "HTTP call failed for " << endpoint;
        throw std::runtime_error(oss.str());
    }

    APIResponse response;
    response.status = request->status;
    response.body = request->body;
    return response;

}

APIResponse HttpLibWrap::post(const   std::string_view& endpoint, json body) {
    //need to add headers, post
    std::cout << "Properly starting post call jere" << std::endl;

    std::string stringBody = body.dump();
    std::cout << std::string(endpoint) << stringBody << "application/json" << std::endl;
    auto request = cli.Post(std::string(endpoint), stringBody, "application/json");

    //if it failed
    if(!request){
        auto err = request.error(); 
        std::ostringstream oss;
        oss << "HTTP call failed for " << endpoint;
        throw std::runtime_error(oss.str());
    }
    //only one use case for post atm but can change accordingly
    std::cout << request->body << std::endl;
    APIResponse response;
    response.status = request->status;
    response.body = request->body;
    return response;


}
void HttpLibWrap::updateAccountSubType(const AccountSubType& type) noexcept{
    dotenv::init();
    std::string credentials;
    //set account type
    //NEED TO PUT TRYS ROUND HERE 
    switch(type){
        case AccountSubType::Stocks:
            credentials = std::string(std::getenv("API_KEY_IDSTOCKS")) + ":" + std::string(std::getenv("API_SECRET_STOCKS"));
            break;
        default:
            std::cout << "Before setting type" << std::endl;
            credentials = std::string(std::getenv("API_KEY_ID_STOCKSISA")) + ":" + std::string(std::getenv("API_SECRET_STOCKSISA"));
            std::cout << "After setting type" << std::endl;
            break;
    }


    std::string encoded = to_base64(credentials);


    httplib::Headers headers = {{"Authorization", "Basic " + encoded}};

    cli.set_default_headers(headers);

    //timeouts for reading writing and connection
    cli.set_read_timeout(5, 0);
    cli.set_write_timeout(5, 0);
    cli.set_connection_timeout(5, 0); 

    std::cout << "api_host constructed" << std::endl;
}

std::string HttpLibWrap::downloadCSV(const std::string& link){

    int schemeEnd = link.find("://");
    if (schemeEnd == std::string::npos) throw std::runtime_error("Invalid URL");
    
    int hostStart = schemeEnd + 3;
    int pathStart = link.find('/', hostStart);
        
    std::string host = link.substr(hostStart, pathStart - hostStart);
    std::string pathAndQuery = link.substr(pathStart);
    
    httplib::SSLClient cli(host);
    auto res = cli.Get(pathAndQuery.c_str());

    if (res && res->status == 200) {
        std::string filePath = "./data/";
        std::filesystem::path fullPath = filePath + "accountInfo.csv";

        // Create directory if needed
        std::filesystem::create_directories(filePath);

        std::ofstream accountInfoFile(fullPath);
        accountInfoFile << res->body;
        return fullPath;

    }else{ 
        throw std::runtime_error("Failed to download CSV file thrown status " +  (res ? std::to_string(res->status) : "no response"));
    }
}
