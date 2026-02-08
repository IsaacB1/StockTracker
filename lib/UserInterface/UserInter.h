#include "IUserInter.h"
#include <TFT_eSPI.h>
#ifndef HOMEINTER_H
#define HOMEINTER_H

class UserInter : public IUserInter {
    private:
        TFT_eSPI tft;
        uint16_t back_colour;
    public:
        UserInter();
        void onStartup();
        void changeBackColour(const uint16_t newColour);
        void writeText(const char* text, const uint16_t& colour, const int& scale, const int& x, const int& y);
        ~UserInter() = default;
};

#endif