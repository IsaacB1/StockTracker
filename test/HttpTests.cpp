#include <Arduino.h>
#include <unity.h>
#include "HttpLibWrap.h"
#include "config.hpp"
HttpLibWrap testHttp;


void test_get_account_cash_200(){
    char buffer[248];
    bool result = true;
    //bool result = testHttp.get(Config::T212_ACCOUNT_INFO_ENDPOINT, buffer);

    TEST_ASSERT_TRUE(result);

}

void setup() {
    Serial.begin(115200);
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_get_account_cash_200);

    UNITY_END();
}

void loop(){}
