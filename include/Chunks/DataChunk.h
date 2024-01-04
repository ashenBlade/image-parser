//
// Created by ashblade on 04.01.24.
//

#ifndef IMAGE_PARSER_DATACHUNK_H
#define IMAGE_PARSER_DATACHUNK_H

#include <vector>
#include "Chunk.h"

class DataChunk: public Chunk {
private:
    std::unique_ptr<uint8_t[]> _data;
    uint32_t _size;
public:
    DataChunk(std::unique_ptr<uint8_t[]>&& data, uint32_t size);
    ~DataChunk() override;

    DataChunk(DataChunk&& other) noexcept;
    DataChunk& operator=(DataChunk&& other) noexcept;

    DataChunk(const DataChunk&) = delete;
    DataChunk& operator=(const DataChunk&) = delete;

    uint32_t size() const;
    uint8_t* getData() const;

    ChunkType getType() const noexcept override;
};

#endif //IMAGE_PARSER_DATACHUNK_H
