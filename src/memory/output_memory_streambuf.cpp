//
// Created by ashblade on 05.01.24.
//
#include <memory>
#include <cstring>
#include "memory/memory_streambuf.h"

static size_t getNextSize(size_t currentCapacity, size_t requiredResultSize) {
    while ((currentCapacity <<= 2) < requiredResultSize) ;
    return currentCapacity;
}

void OutputMemoryStreambuf::ensureCapacity(size_t requestedFreeSize) {

    // Проверяем, что места еще достаточно
    auto leftFreeSize = _capacity - _size;
    if (requestedFreeSize < leftFreeSize) {
        return;
    }

    // Вычисляем следующий размер массива
    auto nextSize = getNextSize(_capacity, _size + requestedFreeSize);
    // Выделяем нужно количество байт
    auto newDataPtr = std::make_unique<char[]>(nextSize);
    // Копируем старые данные
    std::memcpy(newDataPtr.get(), _data.get(), _size);

    // Обновляем состояние
    _data.swap(newDataPtr);
    _capacity = nextSize;
    setp(_data.get(), _data.get() + _capacity);
}

std::streamsize OutputMemoryStreambuf::xsputn(const char *insertData, std::streamsize insertSize) {
    if (insertSize == 0) {
        return 0;
    }

    ensureCapacity(static_cast<size_t>(insertSize));
    std::memcpy(_data.get() + _size, insertData, insertSize);
    _size += insertSize;

    return insertSize;
}

OutputMemoryStreambuf::OutputMemoryStreambuf(int capacity) : _data(std::make_unique<char[]>(capacity)), _size(0), _capacity(capacity) {
    auto start = _data.get();
    setp(start, start + capacity);
}

OutputMemoryStreambuf::~OutputMemoryStreambuf() {
    _size = _capacity = 0;
}

size_t OutputMemoryStreambuf::size() const {
    return _size;
}

size_t OutputMemoryStreambuf::capacity() const {
    return _capacity;
}

const char *OutputMemoryStreambuf::data() const {
    return _data.get();
}

std::unique_ptr<char[]>&& OutputMemoryStreambuf::releaseData() {
    return std::move(_data);
}

OutputMemoryStreambuf::OutputMemoryStreambuf(OutputMemoryStreambuf &&other) noexcept {
    if (this == &other) {
        return;
    }

    _data.swap(other._data);
    _size = other._size;

    other._size = 0;
    other._data.reset();
}



