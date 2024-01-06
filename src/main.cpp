#include <iostream>
#include <fstream>

#include "png/png_file.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "USAGE: " << argv[0] << " path-to-png-file" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary | std::ios::in);
    if (!file) {
        std::cout << "Ошибка открытия файла";
        return 2;
    }

    auto png = PngFile::parseFile(file);

    std::cout << "Файл успешно спарсен\n\n";

    auto header = png.getHeaderChunk();
    std::cout << "Размер изображения: " << header.getWidth() << "x" << header.getHeight() << " px" << std::endl;

    auto palettes = std::vector<PaletteChunk::Entry>();
    if (png.tryGetPalettes(palettes) && !palettes.empty()) {
        std::cout << "Палитры изображения: " << std::endl;
        for (const auto &item: palettes) {
            std::cout << "\tR = " << std::hex << item.r << "\tG = " << item.g << "\tB = " << item.b << std::endl;
            std::cout << std::dec;
        }
    } else {
        std::cout << "Палитр нет" << std::endl;
    }
    switch (header.getInterlaceType()) {
        case InterlaceType::NoInterlace:
            std::cout << "Чрезстрочная разверстка: -" << std::endl;
            break;
        case InterlaceType::Adam7:
            std::cout << "Чрезстрочная разверстка: Adam7" << std::endl;
            break;
        default:
            std::cout << "Чрезстрочная разверстка: неизвестно" << std::endl;
            break;
    }

    auto& dataChunks = png.getDataChunks();

    std::cout << "Количество DATA чанков: " << dataChunks.size() << std::endl;
    int i = 1;
    for (const auto &dataChunk: dataChunks) {
        std::cout << i++ << ": Размер = " << dataChunk.size() << std::endl;
    }

    auto image = png.decodeImage();
    std::ofstream fi("output.dat", std::ios::binary);
    fi.write(image.data(), static_cast<std::streamsize>(image.size()));
    return 0;
}