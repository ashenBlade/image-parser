#include <iostream>
#include <fstream>

#include "PngFile.h"

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
    std::cout << "Размер изображения: " << header.getWidth() << "x" << header.getHeight() << " px";

    return 0;
}