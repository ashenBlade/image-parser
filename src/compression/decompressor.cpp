//
// Created by ashblade on 05.01.24.
//


#include <stdexcept>
#include <sstream>
#include "compression/decompressor.h"

static void initialize(z_stream& stream) {
    stream = {
            .next_in = Z_NULL,
            .avail_in = 0,
            .zalloc = Z_NULL,
            .zfree = Z_NULL,
            .opaque = Z_NULL,
    };

    auto returnCode = inflateInit(&stream);
    if (returnCode < 0) {
        switch (returnCode) {
            case Z_MEM_ERROR:
                throw std::runtime_error("Недостаточно памяти для инициализации потока zlib");
            case Z_VERSION_ERROR:
                throw std::runtime_error("Ошибка совместимости версий zlib");
            default:
                std::stringstream ss("Неизвестная ошибка: ");
                ss << zError(returnCode);
                throw std::runtime_error(ss.str());
        }
    } else if (returnCode == Z_STREAM_END) {
        throw std::runtime_error("Переданы неверные параметры для функции ининициализации");
    }
}

Decompressor::Decompressor(): _inBuffer(), _outBuffer(), _stream() {
    initialize(_stream);
}

Decompressor::~Decompressor() {
    inflateEnd(&_stream);
}

void Decompressor::decompress(std::istream &input, std::ostream &output) {
    do {
        input.read(_inBuffer, bufferSize);
        if (!(input || input.eof())) {
            throw std::runtime_error("Ошибка при чтении данных из входного потока");
        }

        auto readCount = input.gcount();
        if (readCount == 0) {
            break;
        }

        _stream.avail_in = readCount;
        _stream.next_in = reinterpret_cast<Byte*>(_inBuffer);

        int flush = input.eof()
                ? Z_FINISH
                : Z_NO_FLUSH;

        do {
            _stream.avail_out = bufferSize;
            _stream.next_out = reinterpret_cast<Byte*>(_outBuffer);

            int returnCode = inflate(&_stream, Z_NO_FLUSH);
            if (returnCode < 0) {
                std::stringstream ss("Ошибка при сжатии. Код ошибки: ");
                ss << returnCode << ". Описание ошибки: " << _stream.msg;
                throw std::runtime_error(ss.str());
            }

            auto left = static_cast<long>(bufferSize) - static_cast<long>(_stream.avail_out);
            output.write(_outBuffer, left);
            if (output.bad()) {
                throw std::runtime_error("Ошибка записи в выходной поток");
            }
        } while (_stream.avail_out == 0);

        if (flush == Z_FINISH) {
            break;
        }
    } while (true);
}
