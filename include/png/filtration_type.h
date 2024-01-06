//
// Created by ashblade on 04.01.24.
//

#ifndef IMAGE_PARSER_FILTRATION_TYPE_H
#define IMAGE_PARSER_FILTRATION_TYPE_H

#include <cstdint>

enum class FiltrationType: uint8_t {
    None = 0,
    Sub = 1,
    Up = 2,
    Average = 3,
    Paeth = 4,
};

#endif //IMAGE_PARSER_FILTRATION_TYPE_H
