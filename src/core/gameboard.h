#pragma once

#include "tilegenerator.h"
#include <vector>

namespace tfecore {
    enum MoveDirection { LEFT, UP, RIGHT, DOWN };

    class GameBoard {
    private:
        tfecore::TileGenerator tileGenerator;
        std::vector<std::vector<int>> board;

    public:
        GameBoard(size_t boardSize, size_t startingTiles);

        bool addTile();

        bool collapse(tfecore::MoveDirection direction);

        void print();
    };
}
