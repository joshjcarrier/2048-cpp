#include <iostream>
#include <time.h>
#include "tilegenerator.h"

tfecore::TileGenerator::TileGenerator(size_t boardSize) {
    _boardSize = boardSize;
    std::srand(time(NULL));
}

std::tuple<int, int, int> tfecore::TileGenerator::next() {
    int value = std::rand() % 9 == 0 ? 2 : 1;
    return std::tuple<int, int, int>(std::rand() % _boardSize, std::rand() % _boardSize, value);
}
