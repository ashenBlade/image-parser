//
// Created by ashblade on 04.01.24.
//

#ifndef IMAGE_PARSER_PALETTE_CHUNK_H
#define IMAGE_PARSER_PALETTE_CHUNK_H

#include <vector>
#include "png/chunk.h"

class PaletteChunk: public Chunk {
public:
    struct Entry {
        int8_t r;
        int8_t g;
        int8_t b;
    };

    explicit PaletteChunk(std::vector<Entry>&& entries);

    PaletteChunk(const PaletteChunk& other) = delete;
    PaletteChunk& operator=(const PaletteChunk&) = delete;

    PaletteChunk(PaletteChunk&& other) noexcept;
    PaletteChunk& operator=(PaletteChunk&& other) noexcept;

    size_t getPalettesCount() const noexcept;
    Entry operator[](size_t index) const;

    ~PaletteChunk() override;

    ChunkType getType() const noexcept override;

private:
    std::vector<Entry> _entries;
};

#endif //IMAGE_PARSER_PALETTE_CHUNK_H
