#include <cstdlib>
#include <iomanip>
#include <iostream>

#include "collapsestrategy.h"
#include "gameboard.h"
#include "tilegenerator.h"

tfecore::GameBoard::GameBoard(size_t boardSize, size_t startingTiles) : tileGenerator(boardSize) {
    board = std::vector<std::vector<int>>(boardSize, std::vector<int>(boardSize, 0)); // initialize board to 0

    for (size_t i = 0; i < startingTiles;) {
        if(addTile()) {
            i++;
        }
    }
}

bool tfecore::GameBoard::addTile() {
    std::tuple<int, int, int> tile;
    while (true) {
        tile = tileGenerator.next();
        int xPos = std::get<0>(tile);
        int yPos = std::get<1>(tile);

        if (board[yPos][xPos] > 0) {
            // check if any spot is possible
            bool movePossible = canCollapse();

            if (movePossible) {
                continue;
            }
            else {
                return false;
            }
        }

        break;
    }

    int xPos = std::get<0>(tile);
    int yPos = std::get<1>(tile);
    board[yPos][xPos] = std::get<2>(tile);
    return true;
}

bool tfecore::GameBoard::canCollapse() {
    // check if any spot is possible
    for (const auto &row : board) {
        for (const auto &value : row) {
            if (value <= 0) {
                return true;
            }
        }
    }

    return false;
}

bool tfecore::GameBoard::collapse(MoveDirection direction) {
    tfecore::CollapseStrategy cs;
    switch (direction) {
        case tfecore::MoveDirection::DOWN:
            return cs.collapseDown(board);
        case tfecore::MoveDirection::LEFT:
            return cs.collapseLeft(board);
        case tfecore::MoveDirection::RIGHT:
            return cs.collapseRight(board);
        case tfecore::MoveDirection::UP:
            return cs.collapseUp(board);
        default:
            return false;
    }
}

void tfecore::GameBoard::print() {

    // yeah, let's rebuild the string every print. good idea.
    std::string rowBorder(" +");
    for (size_t i = 0; i < board[0].size(); i++) {
        rowBorder += std::string("-------+");
    }

    std::cout << rowBorder << std::endl;
    for (const auto &row : board) {
        for (const auto &value : row) {
            int val = value > 0 ? (1 << value) : 0;
            std::cout << " | " << std::setw(5) << val;
        }

        std::cout << " |" << std::endl;
        std::cout << rowBorder << std::endl;
    }

    std::cout << std:: endl;
}
