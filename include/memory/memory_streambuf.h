//
// Created by ashblade on 05.01.24.
//

#ifndef IMAGE_PARSER_MEMORY_STREAMBUF_H
#define IMAGE_PARSER_MEMORY_STREAMBUF_H

#include <streambuf>
#include <memory>
#include <cstring>

class InputMemoryStreambuf: public std::streambuf {
public:
    InputMemoryStreambuf(char* data, size_t size);
};

class OutputMemoryStreambuf: public std::streambuf {
public:
    explicit OutputMemoryStreambuf(int capacity);
    ~OutputMemoryStreambuf() override;

    OutputMemoryStreambuf(OutputMemoryStreambuf&& other) noexcept;
    OutputMemoryStreambuf& operator=(OutputMemoryStreambuf&& other) noexcept;

    OutputMemoryStreambuf(const OutputMemoryStreambuf&) = delete;
    OutputMemoryStreambuf& operator=(const OutputMemoryStreambuf&) = delete;


    std::unique_ptr<char[]>&& releaseData();
    size_t size() const;
    size_t capacity() const;
    const char* data() const;
private:
    std::unique_ptr<char[]> _data;
    size_t _size;
    size_t _capacity;
    void ensureCapacity(size_t requestedFreeSize);
protected:
    std::streamsize xsputn(const char *insertData, std::streamsize insertSize) override;
};

#endif //IMAGE_PARSER_MEMORY_STREAMBUF_H
