//
// Created by ashblade on 03.01.24.
//

#include <sstream>
#include "png/chunks/header_chunk.h"

HeaderChunk::HeaderChunk(uint32_t height, uint32_t width, uint8_t bitDepth, HeaderChunk::ColorType colorType,
                         CompressionMethod compressionMethod, InterlaceType interlaceType):
                         _height(height), _width(width), _bitDepth(bitDepth), _colorType(colorType),
                         _compressionMethod(compressionMethod), _interlaceType(interlaceType) {
    if (ColorType::AlphaChannel < colorType) {
        std::stringstream ss;
        ss << "Неизвестный тип цвета: " << static_cast<uint8_t>(colorType);
        throw std::runtime_error(ss.str());
    }

    if (CompressionMethod::Deflate < compressionMethod) {
        std::stringstream ss;
        ss << "Неизвестный тип сжатия: " << static_cast<uint8_t>(compressionMethod);
        throw std::runtime_error(ss.str());
    }

    if (InterlaceType::Adam7 < interlaceType) {
        std::stringstream ss;
        ss << "Неизвестный тип чресстрочной развертки: " << static_cast<uint8_t>(interlaceType);
        throw std::runtime_error(ss.str());
    }
}


InterlaceType HeaderChunk::getInterlaceType() const {
    return _interlaceType;
}

CompressionMethod HeaderChunk::getCompressionMethod() const {
    return _compressionMethod;
}

uint32_t HeaderChunk::getHeight() const {
    return _height;
}

uint32_t HeaderChunk::getWidth() const {
    return _width;
}

uint8_t HeaderChunk::getBitDepth() const {
    return _bitDepth;
}

HeaderChunk::ColorType HeaderChunk::getColorType() const {
    return _colorType;
}

HeaderChunk::~HeaderChunk() {
    _width = 0;
    _height = 0;
    _bitDepth = 0;
    _interlaceType = InterlaceType::NoInterlace;
    _colorType = static_cast<ColorType>(0);
    _compressionMethod = CompressionMethod::Deflate;
}

ChunkType HeaderChunk::getType() const noexcept {
    return ChunkType::Header;
}

HeaderChunk::HeaderChunk(const HeaderChunk &other) noexcept {
    if (this == &other) {
        return;
    }

    _width = other._width;
    _height = other._height;
    _bitDepth = other._bitDepth;
    _interlaceType = other._interlaceType;
    _colorType = other._colorType;
    _compressionMethod = other._compressionMethod;
}

HeaderChunk &HeaderChunk::operator=(const HeaderChunk &other) noexcept {
    if (this == &other) {
        return *this;
    }

    _width = other._width;
    _height = other._height;
    _bitDepth = other._bitDepth;
    _interlaceType = other._interlaceType;
    _colorType = other._colorType;
    _compressionMethod = other._compressionMethod;
    return *this;
}

