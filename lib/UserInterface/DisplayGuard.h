#include <memory>
#include "UserInter.h"
#include <TFT_eSPI.h>

class DisplayGuard {
private:
    UserInter& ui;
    std::unique_ptr<TFT_eSprite> sprite;
    int x, y;
public:
    DisplayGuard(UserInter& ui, const char* text, uint16_t colour, int scale, int x, int y, int delay)
        : ui(ui), x(x), y(y), sprite(ui.displayText(text, colour, scale, x, y, delay)) {}
    ~DisplayGuard() { ui.endDisplayText(std::move(sprite), x, y); }
};