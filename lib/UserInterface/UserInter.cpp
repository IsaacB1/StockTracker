#include "IUserInter.h"
#include "UserInter.h"
#include "TextTable.h"
#include <TFT_eSPI.h>

inline void initTextTable() {
    text_table['H'] = H;
}

UserInter::UserInter(){
    initTextTable();

    this->tft = TFT_eSPI();
    tft.init();
    tft.setRotation(1);
    this->changeBackColour(TFT_BLACK);
    this->onStartup();
}

void UserInter::changeBackColour(const uint16_t newColour){
    this->back_colour = newColour;
    this->tft.fillScreen(back_colour);
}


void UserInter::writeText(const char* text, const uint16_t& colour, const int& scale, const int& x, const int& y){
    const int letter_space = 1;
    int xOffset = 0;

    TFT_eSprite sprite = TFT_eSprite(&this->tft);
    const int letter_height = 5;
    const int letter_width = 5 + letter_space;
    sprite.createSprite(scale * letter_width * strlen(text), scale*letter_height);
    sprite.fillSprite(this->back_colour);

    while(*text != '\0'){
        char c = *text;
        c = (char)toupper((unsigned char)c);
        auto letter = (text_table[(uint8_t)c]);
        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 5; c++) {
                if (letter[r][c]) {
                    sprite.fillRect(
                    xOffset + c * scale,
                    r * scale,
                    scale,
                    scale,
                    TFT_WHITE
                    );
                    sprite.pushSprite(x, y);
                    delay(50);
                }
            }
        }
        xOffset += 5 *scale + letter_space;
        text++;
    }
    delay(1000);
    sprite.deleteSprite();
}

void UserInter::onStartup(){
    const char* text = "HHHHHH";
    const int scale = 3;
    writeText(text,TFT_WHITE,scale,100,100);
}