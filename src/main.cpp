#include <iostream>
#include "HttpLibWrap.h"
#include <string>
#include <dotenv.h>


int main() {
    dotenv::init();

    std::string accountInfoEndpoint = "/equity/account/cash";

    std::cout << "Hello" << std::endl;
    const char* api_host = std::getenv("API_HOST");
    
    if(!api_host){
        std::cout << "failed to get from .env file" << std::endl;
        return 1;
    }else{
        HttpLibWrap call = HttpLibWrap(api_host);

        APIResponse response;
        response = call.get(accountInfoEndpoint);
        std::cout << response.status;
        std::cout << response.body;
        return 0;
    }
}