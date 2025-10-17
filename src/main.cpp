#include <iostream>
#include "HttpLibWrap.h"
#include <string>
#include <dotenv.h>
#include <string_view>


int main() {
    dotenv::init();

    constexpr std::string_view StocksISAEnpoint = "/api/v0/equity/account/cash";

    std::cout << "Hello" << std::endl;
    const char* api_host = std::getenv("API_HOST");
    
    if(!api_host){
        std::cout << "failed to get from .env file" << std::endl;
        return 1;
    }else{
            HttpLibWrap call = HttpLibWrap(api_host);
        try{
            APIResponse response;
            response = call.get(StocksISAEnpoint);
            std::cout << response.status;
            std::cout << response.body;
        }catch(const std::runtime_error e){
            std::cerr << "Caught exception: " << e.what() << std::endl;
            return 1;
        }

        return 0;
    }
}