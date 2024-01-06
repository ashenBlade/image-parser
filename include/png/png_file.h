#ifndef IMAGE_PARSER_PNG_FILE_H
#define IMAGE_PARSER_PNG_FILE_H

#include <iostream>
#include <vector>
#include <memory>
#include "chunk.h"
#include "png/chunks/header_chunk.h"
#include "png/chunks/palette_chunk.h"
#include "png/chunks/data_chunk.h"
#include "png_image.h"

class PngFile {
private:
    HeaderChunk _header;
    std::unique_ptr<PaletteChunk> _palette;
    std::vector<DataChunk> _dataChunks;

    PngFile(HeaderChunk& header, std::unique_ptr<PaletteChunk>&& palette, std::vector<DataChunk>&& dataChunks);
public:
    // Запрещаем лишнее копирование
    PngFile(const PngFile&) = delete;
    PngFile& operator=(const PngFile&) = delete;

    // А вот перемещение - вполне ОК
    PngFile& operator=(PngFile&&) noexcept;
    PngFile(PngFile&&) noexcept;

    HeaderChunk getHeaderChunk() const;
    bool tryGetPalettes(std::vector<PaletteChunk::Entry>& entries);

    static PngFile parseFile(std::istream& input);

    const std::vector<DataChunk>& getDataChunks() const;

    PngImage decodeImage() const;
};


#endif //IMAGE_PARSER_PNG_FILE_H
