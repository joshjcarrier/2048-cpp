#include <iostream>
#include <time.h>
#include "tilegenerator.h"

tfecore::TileGenerator::TileGenerator() {
    std::srand(time(NULL));
}

std::tuple<int, int, int> tfecore::TileGenerator::next() {
    int value = std::rand() % 9 == 0 ? 2 : 1;
    return std::tuple<int, int, int>(std::rand() % 4, std::rand() % 4, value);
}
