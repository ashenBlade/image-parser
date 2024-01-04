#ifndef IMAGE_PARSER_PNGFILE_H
#define IMAGE_PARSER_PNGFILE_H

#include <iostream>
#include <vector>
#include <memory>
#include "Chunk.h"
#include "Chunks/HeaderChunk.h"
#include "Chunks/PaletteChunk.h"

class PngFile {
private:
    HeaderChunk _header;
    std::unique_ptr<PaletteChunk> _palette;

    PngFile(HeaderChunk& header, std::unique_ptr<PaletteChunk>&& palette);
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
};


#endif //IMAGE_PARSER_PNGFILE_H
