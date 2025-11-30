/* This is the average return from Trading 212 API 
[
    {
        "averagePrice": 0,
        "currentPrice": 0,
        "frontend": "API",
        "fxPpl": 0,
        "initialFillDate": "2019-08-24T14:15:22Z",
        "maxBuy": 0,
        "maxSell": 0,
        "pieQuantity": 0,
        "ppl": 0,
        "quantity": 0,
        "ticker": "AAPL_US_EQ"
    }
]*/
#include <string>
#ifndef ISTOCK_H
#define ISTOCK_H

class IPortfolioStats{

    public:
        virtual ~IPortfolioStats() = default;
};

#endif
