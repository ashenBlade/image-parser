//
// Created by ashblade on 05.01.24.
//

#ifndef IMAGE_PARSER_DECOMPRESSOR_H
#define IMAGE_PARSER_DECOMPRESSOR_H

#include "zlib.h"

class Decompressor {
private:
    constexpr static const int bufferSize = 4096; // 4Кб
    char _inBuffer[bufferSize];
    char _outBuffer[bufferSize];
    z_stream _stream;
public:
    Decompressor();
    ~Decompressor();

    Decompressor(Decompressor&&) = delete;
    Decompressor& operator=(Decompressor&&) = delete;

    Decompressor(const Decompressor&) = delete;
    Decompressor& operator=(const Decompressor&) = delete;

    void decompress(std::istream& source, std::ostream& dest);
};

#endif //IMAGE_PARSER_DECOMPRESSOR_H
