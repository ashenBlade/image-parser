//
// Created by ashblade on 05.01.24.
//

#include <memory>
#include <system_error>

#include "png/png_image.h"


PngImage::PngImage(std::unique_ptr<char[]> &&bytes, size_t length): _rawBytes(std::move(bytes)), _length(length)
{  }

size_t PngImage::size() const {
    return _length;
}

char *PngImage::data() const {
    return _rawBytes.get();
}

PngImage::~PngImage() {
    _length = 0;
}

PngImage::PngImage(PngImage &&other) noexcept {
    if (this == &other) {
        return;
    }

    _rawBytes.swap(other._rawBytes);
    _length = other._length;
}

PngImage &PngImage::operator=(PngImage &&other)  noexcept {
    if (this == &other) {
        return *this;
    }

    _rawBytes.swap(other._rawBytes);
    _length = other._length;
    return *this;
}

