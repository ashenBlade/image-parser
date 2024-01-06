//
// Created by ashblade on 04.01.24.
//

#include "png/chunks/data_chunk.h"
#include "memory/memory_streambuf.h"
#include "compression/decompressor.h"

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

size_t DataChunk::size() const {
    return _size;
}

char *DataChunk::data() const {
    return _data.get();
}

ChunkType DataChunk::getType() const noexcept {
    return ChunkType::Data;
}

DataChunk::~DataChunk() {
    _size = 0;
}

DataChunk::DataChunk(std::unique_ptr<char[]> &&data, size_t size): _data(std::move(data)), _size(size) {  }

static OutputMemoryStreambuf decodeSingleChunk(char* compressedData, size_t compressedLength) {
    InputMemoryStreambuf inBuffer(compressedData, compressedLength);
    OutputMemoryStreambuf outBuffer(8192); // Такого размера были чанки в изображении, на котором проводил тесты

    std::istream input(&inBuffer);
    std::ostream output(&outBuffer);

    Decompressor decompressor;
    decompressor.decompress(input, output);

    // Теперь нужно применить сжатие
    return outBuffer;
}

