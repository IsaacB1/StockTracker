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
constexpr uint8_t ROW_TWO_1[5] = {0, 1, 0, 1, 0};
constexpr uint8_t ROW_TWO_2[5] = {1, 1, 0, 0, 0};
constexpr uint8_t ROW_TWO_3[5] = {1, 0, 1, 0, 0};
constexpr uint8_t ROW_TWO_4[5] = {1, 0, 0, 1, 0};

//3 BIT MAPS
const uint8_t ROW_THREE_1[5] = {1, 1, 1, 0, 0};
const uint8_t ROW_THREE_2[5] = {1, 1, 0, 0, 1};
const uint8_t ROW_THREE_3[5] = {1, 0, 1, 0, 1};
const uint8_t ROW_THREE_4[5] = {1, 0, 0, 1, 1};

constexpr uint8_t ROW_FOUR_1[5] = {1, 1, 1, 1, 0};
constexpr uint8_t ROW_FOUR_2[5] = {1, 0, 1, 1, 1};
constexpr uint8_t ROW_FOUR_3[5] = {1, 1, 0, 1, 1};


const uint8_t* A[5] = {
    ROW_FULL,
    ROW_SIDES,
    ROW_FULL,
    ROW_SIDES,
    ROW_SIDES
};

const uint8_t* C[5] = {
    ROW_FULL,
    ROW_ONE_1,
    ROW_ONE_1,
    ROW_ONE_1,
    ROW_FULL
};

const uint8_t* D[5] = {
    ROW_FOUR_1,
    ROW_SIDES,
    ROW_SIDES,
    ROW_SIDES,
    ROW_FOUR_1
};

const uint8_t* E[5] = {
    ROW_FULL,
    ROW_ONE_1,
    ROW_FULL,
    ROW_ONE_1,
    ROW_FULL
};

const uint8_t* F[5] = {
    ROW_FULL,
    ROW_ONE_1,
    ROW_THREE_1,
    ROW_ONE_1,
    ROW_ONE_1
};

const uint8_t* G[5] = {
    ROW_FULL,
    ROW_ONE_1,
    ROW_FOUR_2,
    ROW_SIDES,
    ROW_FULL
};

const uint8_t* H[5] = {
    ROW_SIDES,
    ROW_SIDES,
    ROW_FULL,
    ROW_SIDES,
    ROW_SIDES
};

const uint8_t* I[5] = {
    ROW_FULL,
    ROW_ONE_3,
    ROW_ONE_3,
    ROW_ONE_3,
    ROW_FULL
};

const uint8_t* K[5] = {
    ROW_SIDES,
    ROW_TWO_4,
    ROW_TWO_2,
    ROW_TWO_4,
    ROW_SIDES
};

const uint8_t* L[5] = {
    ROW_ONE_1,
    ROW_ONE_1,
    ROW_ONE_1,
    ROW_ONE_1,
    ROW_FULL
};

const uint8_t* M[5] = {
    ROW_SIDES,
    ROW_FOUR_3,
    ROW_THREE_3,
    ROW_SIDES,
    ROW_SIDES
};

const uint8_t* N[5] = {
    ROW_SIDES,
    ROW_THREE_2,
    ROW_THREE_3,
    ROW_THREE_4,
    ROW_SIDES
};

const uint8_t* O[5] = {
    ROW_FULL,
    ROW_SIDES,
    ROW_SIDES,
    ROW_SIDES,
    ROW_FULL
};

const uint8_t* T[5] = {
    ROW_FULL,
    ROW_ONE_3,
    ROW_ONE_3,
    ROW_ONE_3,
    ROW_ONE_3
};

const uint8_t* U[5] = {
    ROW_SIDES,
    ROW_SIDES,
    ROW_SIDES,
    ROW_SIDES,
    ROW_FULL
};


//(dot)
const uint8_t* PERIOD[5] = {
    ROW_EMPTY,
    ROW_EMPTY,
    ROW_EMPTY,
    ROW_ONE_3,
    ROW_EMPTY
};

const uint8_t* SPACE[5] = {
    ROW_EMPTY,
    ROW_EMPTY,
    ROW_EMPTY,
    ROW_EMPTY,
    ROW_EMPTY
};

const uint8_t* EXCLAMATION[5] = {
    ROW_ONE_3,
    ROW_ONE_3,
    ROW_ONE_3,
    ROW_EMPTY,
    ROW_ONE_3
};

const uint8_t** text_table[128] = { nullptr }; 


#endif