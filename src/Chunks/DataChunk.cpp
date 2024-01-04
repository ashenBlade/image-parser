//
// Created by ashblade on 04.01.24.
//

#include "Chunks/DataChunk.h"

DataChunk::DataChunk(DataChunk &&other) noexcept {
    if (this == &other) {
        return;
    }

    _data.swap(other._data);
    _size = other._size;

    other._data.reset();
    other._size = 0;
}

DataChunk &DataChunk::operator=(DataChunk &&other) noexcept {
    if (this == &other) {
        return *this;
    }

    _data.swap(other._data);
    _size = other._size;

    other._data.reset();
    other._size = 0;
    return *this;
}

uint32_t DataChunk::size() const {
    return _size;
}

uint8_t *DataChunk::getData() const {
    return _data.get();
}

ChunkType DataChunk::getType() const noexcept {
    return ChunkType::Data;
}

DataChunk::~DataChunk() {
    _size = 0;
}

DataChunk::DataChunk(std::unique_ptr<uint8_t[]> &&data, uint32_t size) {
    _data = std::move(data);
    _size = size;
}
