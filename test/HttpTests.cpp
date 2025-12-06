#include <iostream>
#include <unity.h>
#include "HttpLibWrap.h"
//#include "config.hpp"
//HttpLibWrap testHttp;


void test_get_account_cash_200(){
    char buffer[248];
    //bool result = testHttp.get(Config::T212_ACCOUNT_INFO_ENDPOINT, buffer);

    std::cout << buffer;
    TEST_ASSERT_TRUE(true);

}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_get_account_cash_200);

    UNITY_END();

    return 0;
}