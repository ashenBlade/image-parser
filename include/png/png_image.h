//
// Created by ashblade on 05.01.24.
//

#ifndef IMAGE_PARSER_PNG_IMAGE_H
#define IMAGE_PARSER_PNG_IMAGE_H

class PngImage {
private:
    std::unique_ptr<char[]> _rawBytes;
    size_t _length;

public:
    PngImage(std::unique_ptr<char[]>&& bytes, size_t length);
    ~PngImage();

    PngImage(PngImage&& other) noexcept ;
    PngImage& operator=(PngImage&& other) noexcept;

    PngImage(const PngImage&) = delete;
    PngImage& operator=(const PngImage&) = delete;

    size_t size() const;
    char* data() const;
};

#endif //IMAGE_PARSER_PNG_IMAGE_H
