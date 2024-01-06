//
// Created by ashblade on 03.01.24.
//

#ifndef IMAGE_PARSER_CHUNK_TYPE_H
#define IMAGE_PARSER_CHUNK_TYPE_H

enum class ChunkType {
    // Основные
    Header = 0,
    Palette,
    Data,
    End,

    // Цвета изображения
    BackgroundColor,
    CieColorSpace,
    Srgb,
    UsagePalette,
    IccColorProfile,

    // Метаинформация об изображении
    Gamma,
    Histogram,
    StereoscopicImage,
    PixelSize,
    ColorAccuracy,
    LastImageChange,
    TransparencyInfo,

    // Текст
    UtfText,
    IsoText,
    CompressedTest
};

#endif //IMAGE_PARSER_CHUNK_TYPE_H
