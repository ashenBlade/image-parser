//
// Created by ashblade on 03.01.24.
//

#include "PngFile.h"
#include "Chunks/PaletteChunk.h"

#include <system_error>
#include <cstring>
#include <sstream>

constexpr const int HeaderSize = 8;
constexpr const int CrcSize = 4;

constexpr static const char pngFileHeader[HeaderSize] = {
        (char) 0x89, // Non-ASCII
        'P', 'N', 'G',
        0x0D, 0x0A, // CRLF (DOS)
        0x1A, // EOF (DOS)
        0x0A // LF (UNIX)
};

static void skipCrc(std::istream &input) {
    input.seekg(CrcSize, std::ios::cur);
}

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

    int width = readInt32BigEndian(input);
    int height = readInt32BigEndian(input);

    uint8_t bitDepth, colorType, compressionMethod, filterMethod, interlaceMethod;
    input.read(reinterpret_cast<char*>(&bitDepth), 1);
    input.read(reinterpret_cast<char*>(&colorType), 1);
    input.read(reinterpret_cast<char*>(&compressionMethod), 1);
    input.read(reinterpret_cast<char*>(&filterMethod), 1);
    input.read(reinterpret_cast<char*>(&interlaceMethod), 1);
    if (!input) {
        throw std::runtime_error("Ошибка при чтении данных из потока");
    }

    return HeaderChunk(height, width,
                       bitDepth,
                       static_cast<HeaderChunk::ColorType>(colorType),
                       static_cast<CompressionMethod>(compressionMethod),
                       static_cast<InterlaceType>(interlaceMethod));
}

static std::unique_ptr<PaletteChunk> parsePaletteChunk(std::istream &input, int dataLength) {
    if (dataLength % 3 != 0) {
        std::stringstream ss;
        ss << "Размер данных в чанке палитры не кратен 3. Прочитан размер: " << dataLength;
        throw std::runtime_error(ss.str());
    }
    
    std::vector<PaletteChunk::Entry> entries(dataLength / 3);
    for (auto &entry: entries) {
        char buf[3];
        input.read(buf, 3);
        entry = PaletteChunk::Entry {
            .r = buf[0],
            .g = buf[1],
            .b = buf[2],
        };
    }

    // Пропускаем CRC
    skipCrc(input);

    return std::make_unique<PaletteChunk>(std::move(entries));
}

class Prefix {
public:
    constexpr static const char Header[] = "IHDR";
    constexpr static const char Palette[] = "PLTE";
    constexpr static const char End[] = "IEND";
};

constexpr const char Prefix::Header[];
constexpr const char Prefix::Palette[];
constexpr const char Prefix::End[];

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

    if (strcmp(Prefix::Header, chunkType) != 0) {
        std::stringstream ss;
        ss << "Неизвестный тип чанка: " << chunkType << ". Пока поддерживается только заголовок";
        throw std::runtime_error(ss.str());
    }

    if (chunkLength != 13) {
        std::stringstream ss;
        ss << "Размер чанка заголовка должен быть равен 13. Прочитано: " << chunkLength;
        throw std::runtime_error(ss.str());
    }

    HeaderChunk header = parseHeaderChunk(input);
    std::unique_ptr<PaletteChunk> palette;

    skipCrc(input);
    int iter = 0;

    while (!input.eof()) {
        chunkLength = readInt32BigEndian(input);
        if (!input) {
            std::stringstream ss;
            ss << "Ошибка чтения длины чанка: ";
            const char* detail;
            if (input.eof()) {
                detail = "файл закончился";
                ss << "Последний тип чанка: " << iter << ' ';
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

        input.read(chunkType, 4);
        if (!input) {
            throw std::runtime_error("Ошибка чтения типа чанка");
        }

        if (std::strcmp(Prefix::Palette, chunkType) == 0) {
            palette = std::move(parsePaletteChunk(input, chunkLength));
        } else if (std::strcmp(Prefix::End, chunkType) == 0) {
            skipCrc(input);
            break;
        } else {
            // Пропускаем чанк
            input.seekg(chunkLength + CrcSize, std::ios::cur);
            if (!input) {
                throw std::runtime_error("Ошибка после смещения позиции файла для неизвесного чанка");
            }
        }
        iter++;
    }

    return PngFile(header, std::move(palette));
}


PngFile::PngFile(PngFile && other) noexcept: _header(HeaderChunk(0, 0, 0,
                                                                 static_cast<HeaderChunk::ColorType>(0),
                                                                 static_cast<CompressionMethod>(0),
                                                                 static_cast<InterlaceType>(0))) {
    if (this == &other) {
        return;
    }

    _header = other._header;
    _palette = std::move(other._palette);
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

PngFile::PngFile(HeaderChunk &header, std::unique_ptr<PaletteChunk>&& palette): _header(header), _palette(std::move(palette))
{ }

bool PngFile::tryGetPalettes(std::vector<PaletteChunk::Entry> &entries) {
    if (_palette) {
        auto count = _palette->getPalettesCount();
        for (size_t i = 0; i < count; ++i) {
            entries.push_back(_palette->operator[](i));
        }
        return true;
    }
    return false;
}



