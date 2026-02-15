#ifndef TEXTTABLE_H
#define TEXTTABLE_H
#include <cstdint>

//could be imporived by definingneach row
constexpr uint8_t ROW_EMPTY[5] = {0, 0, 0, 0, 0};
constexpr uint8_t ROW_FULL[5] = {1, 1, 1, 1, 1};

//One bit maps
constexpr uint8_t ROW_ONE_1[5] = {1, 0, 0, 0, 0};
constexpr uint8_t ROW_ONE_2[5] = {0, 1, 0, 0, 0};
constexpr uint8_t ROW_ONE_3[5] = {0, 0, 1, 0, 0};
constexpr uint8_t ROW_ONE_4[5] = {0, 0, 0, 1, 0};
constexpr uint8_t ROW_ONE_5[5] = {0, 0, 0, 0, 1};

//Two bit maps
constexpr uint8_t ROW_SIDES[5] = {1, 0, 0, 0, 1};

const uint8_t* E[5] = {
    ROW_FULL,
    ROW_ONE_1,
    ROW_FULL,
    ROW_ONE_1,
    ROW_FULL
};

const uint8_t* H[5] = {
    ROW_SIDES,
    ROW_SIDES,
    ROW_FULL,
    ROW_SIDES,
    ROW_SIDES
};

const uint8_t* L[5] = {
    ROW_ONE_1,
    ROW_ONE_1,
    ROW_ONE_1,
    ROW_ONE_1,
    ROW_FULL
};

const uint8_t* O[5] = {
    ROW_FULL,
    ROW_SIDES,
    ROW_SIDES,
    ROW_SIDES,
    ROW_FULL
};

const uint8_t** text_table[128] = { nullptr }; 


#endif