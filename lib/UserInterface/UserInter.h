#include "IUserInter.h"
#include <TFT_eSPI.h>
#include <memory>

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
        // note: these operate on the sprite itself; callers manage the
        // unique_ptr that owns it.  Passing by reference avoids moving
        // ownership and prevents the object from being destroyed mid-use.
        void writeText(TFT_eSprite &sprite, const char* text, const uint16_t& colour, const int& scale, const int& x, const int& y, const int& local_delay);
        bool cleanUpSprite(TFT_eSprite &sprite, const int& x, const int& y);
        ~UserInter() = default;
        bool endLoading(std::unique_ptr<TFT_eSprite> loading_sprite);
        std::unique_ptr<TFT_eSprite> startLoading();
        std::unique_ptr<TFT_eSprite> displayText(const char* text, const uint16_t& colour, const int& scale, const int& x, const int& y, const int& local_delay);
        bool endDisplayText(std::unique_ptr<TFT_eSprite> display_text, const int& x, const int& y);
};
#endif