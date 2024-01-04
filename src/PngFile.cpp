//
// Created by ashblade on 03.01.24.
//

#include "../include/PngFile.h"

#include <system_error>
#include <cstring>
#include <sstream>

const int HeaderSize = 8;

constexpr static const char pngFileHeader[HeaderSize] = {
        (char) 0x89, // Non-ASCII
        'P', 'N', 'G',
        0x0D, 0x0A, // CRLF (DOS)
        0x1A, // EOF (DOS)
        0x0A // LF (UNIX)
};

static void validatePngHeader(std::istream& file) {
    char buf[HeaderSize];

    // Проверяем заголовок
    file.read(buf, HeaderSize);
    if (!file) {
        throw std::runtime_error("Не удалось полностью прочитать заголовок файла");
    }

    if (memcmp(buf, pngFileHeader, HeaderSize) != 0) {
        throw std::runtime_error("Прочитанный заголовок не равен заголовку PNG файла");
    }
}

static inline int readInt32BigEndian(std::istream &input) {
    int value = 0;
    input.read(reinterpret_cast<char*>(&value), sizeof(int));
    return be32toh(value);
}

static HeaderChunk parseHeaderChunk(std::istream &input) {
    uint8_t bitDepth, colorType, compressionMethod, filterMethod, interlaceMethod;

    int width = readInt32BigEndian(input);
    int height = readInt32BigEndian(input);

    // Читаем обычные байты
    input >> bitDepth >> colorType >> compressionMethod >> filterMethod >> interlaceMethod;

    if (!input) {
        std::stringstream ss;
        ss << "Ошибка при чтении данных из потока";
        throw std::runtime_error(ss.str());
    }

    return HeaderChunk(height, width, bitDepth,
                       static_cast<HeaderChunk::ColorType>(colorType),
                       static_cast<CompressionMethod>(compressionMethod),
                       static_cast<InterlaceType>(interlaceMethod));
}



class Prefix {
public:
    constexpr static const char Header[] = "IHDR";
};

constexpr const char Prefix::Header[];

PngFile PngFile::parseFile(std::istream &input) {
    // Проверяем заголовок самого файла
    validatePngHeader(input);

    // Начинаем парсить чанки файла
    int chunkLength;
    char chunkType[4];

    chunkLength = readInt32BigEndian(input);
    if (!input) {
        std::stringstream ss;
        ss << "Ошибка чтения длины чанка заголовка: ";
        const char* detail;
        if (input.eof()) {
            detail = "файл закончился";
        } else if (input.bad()) {
            detail = "выставлен флаг bad";
        } else if (input.fail()) {
            detail = "выставлен флайг fail";
        } else if (input.good()) {
            detail = "файл ок, но читать дальше ошибка";
        } else {
            detail = "неизвестная ошибка";
        }
        ss << detail;
        throw std::runtime_error(ss.str());
    }

    input.read(chunkType, sizeof(chunkType));
    if (!input) {
        throw std::runtime_error("Ошибка чтения типа чанка заголовка");
    }

    if (strcmp(Prefix::Header, chunkType) == 0) {
        if (chunkLength != 13) {
            std::stringstream ss;
            ss << "Размер чанка заголовка должен быть равен 13. Прочитано: " << chunkLength;
            throw std::runtime_error(ss.str());
        }

        auto header = parseHeaderChunk(input);
        return PngFile(header);
    }

    std::stringstream ss;
    ss << "Неизвестный тип чанка: " << chunkType << ". Пока поддерживается только заголовок";
    throw std::runtime_error(ss.str());
}


PngFile::PngFile(PngFile && other) noexcept: _header(other._header) {
    if (this == &other) {
        return;
    }

    _header = other._header;
}

PngFile &PngFile::operator=(PngFile &&other) noexcept {
    if (this == &other) {
        return *this;
    }

    _header = other._header;
    return *this;
}

HeaderChunk PngFile::getHeaderChunk() const {
    return _header;
}

PngFile::PngFile(HeaderChunk &header): _header(header) { }



