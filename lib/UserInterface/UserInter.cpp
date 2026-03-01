#include "IUserInter.h"
#include "UserInter.h"
#include "TextTable.h"
#include <TFT_eSPI.h>
#include <memory>

inline void initTextTable() {
    text_table['H'] = H;
    text_table['E'] = E;
    text_table['L'] = L;
    text_table['O'] = O;
    text_table['!'] = EXCLAMATION;
    // letters required for the loading screen
    text_table['A'] = A;
    text_table['D'] = D;
    text_table['I'] = I;
    text_table['N'] = N;
    text_table['G'] = G;
    text_table['.'] = PERIOD;
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

void UserInter::writeText(TFT_eSprite &sprite, const char* text, const uint16_t& colour, const int& scale, const int& x, const int& y){
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
    uint16_t x = 5;
    bool touched = tft.getTouch(&x, &x);
    Serial.print("Touch Status: ");
    Serial.println(touched ? "ALIVE" : "DEAD"); 
}

std::unique_ptr<TFT_eSprite> UserInter::startLoading(){
    const char* text = "Loading...";
    const int scale = 2;

    auto loading_text = std::make_unique<TFT_eSprite>(&this->tft);
    writeText(*loading_text, text, TFT_WHITE, scale, 100, 100);

    return loading_text;
}

bool UserInter::endLoading(std::unique_ptr<TFT_eSprite> loading_sprite){
    if (loading_sprite && cleanUpSprite(*loading_sprite, 100, 100)) {
        Serial.println("Loading sprite deleted successfully");
        return true;
    }
    return false;
}