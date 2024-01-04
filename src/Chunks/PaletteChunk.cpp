//
// Created by ashblade on 04.01.24.
//

#include "Chunks/PaletteChunk.h"

PaletteChunk::PaletteChunk(std::vector<Entry> &&entries): _entries(std::move(entries)) { }

PaletteChunk::PaletteChunk(PaletteChunk &&other)  noexcept {
    if (this == &other) {
        return;
    }

    _entries = std::move(other._entries);
}

PaletteChunk &PaletteChunk::operator=(PaletteChunk &&other)  noexcept {
    if (this == &other) {
        return *this;
    }

    _entries = std::move(other._entries);
    return *this;
}

size_t PaletteChunk::getPalettesCount() const noexcept {
    return _entries.size();
}

PaletteChunk::Entry PaletteChunk::operator[](size_t index) const {
    return _entries[index];
}

ChunkType PaletteChunk::getType() const noexcept {
    return ChunkType::Palette;
}

PaletteChunk::~PaletteChunk() = default;


