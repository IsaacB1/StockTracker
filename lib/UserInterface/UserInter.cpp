#include "IUserInter.h"
#include "UserInter.h"
#include "TextTable.h"
#include <TFT_eSPI.h>

inline void initTextTable() {
    text_table['H'] = H;
    text_table['E'] = E;
    text_table['L'] = L;
    text_table['O'] = O;
    text_table['!'] = EXCLAMATION;
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

void UserInter::writeText(TFT_eSprite& sprite, const char* text, const uint16_t& colour, const int& scale, const int& x, const int& y){
    const int letter_space = 1;
    int xOffset = 0;

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
                    delay(15);
                }
            }
        }
        xOffset += 5 *scale + letter_space;
        text++;
    }

}

bool UserInter::cleanUpSprite(TFT_eSprite &sprite,const int& x, const int& y){
    if(sprite.created()){
        sprite.fillSprite(this->back_colour);
        sprite.pushSprite(x,y);
        sprite.deleteSprite();
        return true;
    }
    return false;
}


void UserInter::onStartup(){
    const char* text = "HELLO!";
    const int scale = 2;

    TFT_eSprite first_text = TFT_eSprite(&this->tft); 
    writeText(first_text, text,TFT_WHITE,scale,100,100);

    TFT_eSprite second_text = TFT_eSprite(&this->tft);;
    writeText(second_text, text,TFT_WHITE,scale,105,105);

    delay(1000);

    if(cleanUpSprite(first_text,100,100)){Serial.println("First sprite deleted successfully");}
    if(cleanUpSprite(second_text,105,105)){Serial.println("Second sprite deleted successfully");}
}