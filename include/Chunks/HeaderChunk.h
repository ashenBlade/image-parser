//
// Created by ashblade on 04.01.24.
//

#ifndef IMAGE_PARSER_HEADERCHUNK_H
#define IMAGE_PARSER_HEADERCHUNK_H

#include "Chunk.h"
#include "CompressionMethod.h"
#include "InterlaceType.h"

class HeaderChunk: public Chunk {
public:
    ~HeaderChunk() override;

    ChunkType getType() const noexcept override;

    enum class ColorType: uint8_t {
        // Используется палитра
        Palette = 1,
        // Используется цвет (не монохромное)
        Color = 1 << 1,
        // Используется альфа канал
        AlphaChannel = 1 << 2,
    };

    HeaderChunk(uint32_t height,
                uint32_t width,
                uint8_t bitDepth,
                ColorType colorType,
                CompressionMethod compressionMethod,
                InterlaceType interlaceType);

    // Тут никаких данных нет - можем спокойно копировать (это простые поля)
    HeaderChunk(const HeaderChunk& other) noexcept;
    HeaderChunk& operator=(const HeaderChunk& other) noexcept;

    uint32_t getHeight() const;
    uint32_t getWidth() const;
    uint8_t getBitDepth() const;
    ColorType getColorType() const;
    CompressionMethod getCompressionMethod() const;
    InterlaceType getInterlaceType() const;

private:
    uint32_t _height;
    uint32_t _width;
    uint8_t _bitDepth;
    ColorType _colorType;
    CompressionMethod _compressionMethod;
    InterlaceType _interlaceType;
};

#endif //IMAGE_PARSER_HEADERCHUNK_H
