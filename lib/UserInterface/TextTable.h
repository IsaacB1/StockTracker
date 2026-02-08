#ifndef TEXTTABLE_H
#define TEXTTABLE_H
#include <cstdint>

//have to manally assign due to compiler limitations
constexpr uint8_t H[5][5] = {
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1}
};

const uint8_t (*text_table[128])[5] = { nullptr }; 


#endif