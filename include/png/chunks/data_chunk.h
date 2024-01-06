//
// Created by ashblade on 04.01.24.
//

#ifndef IMAGE_PARSER_DATA_CHUNK_H
#define IMAGE_PARSER_DATA_CHUNK_H

#include <vector>
#include "png/chunk.h"
#include "memory/memory_streambuf.h"

class DataChunk: public Chunk {
private:
    std::unique_ptr<char[]> _data;
    size_t _size;
public:
    DataChunk(std::unique_ptr<char[]>&& data, size_t size);
    ~DataChunk() override;

    DataChunk(DataChunk&& other) noexcept;
    DataChunk& operator=(DataChunk&& other) noexcept;

    DataChunk(const DataChunk&) = delete;
    DataChunk& operator=(const DataChunk&) = delete;

    size_t size() const;
    char* data() const;

    ChunkType getType() const noexcept override;
};

#endif //IMAGE_PARSER_DATA_CHUNK_H
