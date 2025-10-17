#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "IAPIClient.h"
#include "base64.h"
#include <cstdlib>
#include "HttpLibWrap.h"
#include <string>

//need to add API secrets now
HttpLibWrap::HttpLibWrap(const std::string& host) : cli(host.c_str()){
    cli.enable_server_certificate_verification(false);
            
    std::string credentials = std::string(std::getenv("API_KEY_ID")) + ":" + std::string(std::getenv("API_SECRET"));

    std::string encoded = to_base64(credentials);


    httplib::Headers headers = {{"Authorization", "Basic " + encoded}};

    cli.set_default_headers(headers);

    //timeouts for reading writing and connection
    cli.set_read_timeout(5, 0);
    cli.set_write_timeout(5, 0);
    cli.set_connection_timeout(5, 0); 

    std::cout << "api_host constructed" << std::endl;
}   
//nmeed to implement requests now
APIResponse HttpLibWrap::get(const std::string& endpoint) {
    std::cout << endpoint << std::endl;

    auto request = cli.Get(endpoint.c_str());

    //if it succeeded
    if(!request){
        auto err = request.error(); 
        std::cerr << "Request failed, error = " << httplib::to_string(err) << std::endl;
        throw std::runtime_error("HTTP call failed for " + endpoint);
    }

    APIResponse response;
    response.status = request->status;
    response.body = request->body;
    return response;

    std::cout << "request failed to construct" << std::endl;
    //if not
    //in future logger erro here
    APIResponse failed;
    failed.status = request->status;
    failed.body = "Request failed";
    return failed;
}
