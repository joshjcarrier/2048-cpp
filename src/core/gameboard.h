#pragma once

#include "tilegenerator.h"
#include <vector>

namespace tfecore {
    enum MoveDirection { LEFT, UP, RIGHT, DOWN };

    class GameBoard {
    private:
        tfecore::TileGenerator tileGenerator;
        std::vector<std::vector<int>> board = std::vector<std::vector<int>>(4, std::vector<int>(4, 0)); // initialize board to 0

    public:
        GameBoard();

        bool addTile();

        bool collapse(tfecore::MoveDirection direction);

        void print();
    };
}
