#pragma once

#include "tilegenerator.h"

namespace tfecore {
    enum MoveDirection { LEFT, UP, RIGHT, DOWN };

    class GameBoard {
    private:
        tfecore::TileGenerator tileGenerator;
        int board[4][4] = {{ 0 }}; // initialize board to 0

    public:
        GameBoard();

        bool addTile();

        void collapse(tfecore::MoveDirection direction);

        void print();
    };
}
