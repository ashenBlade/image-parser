//
// Created by ashblade on 03.01.24.
//

#ifndef IMAGE_PARSER_CHUNK_H
#define IMAGE_PARSER_CHUNK_H

#include <istream>
#include <memory>
#include "ChunkType.h"


class Chunk {
protected:
    Chunk() = default;
public:
    virtual ~Chunk() = 0;

    Chunk(const Chunk& other) = delete;
    Chunk& operator=(const Chunk&) = delete;

    Chunk(Chunk&& other) = delete;
    Chunk& operator=(Chunk&& other) = delete;

    virtual ChunkType getType() const noexcept = 0;
};


#endif //IMAGE_PARSER_CHUNK_H
