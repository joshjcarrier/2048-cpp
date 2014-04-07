#include <cstdlib>
#include <iomanip>
#include <iostream>

#include "collapsestrategy.h"
#include "gameboard.h"
#include "tilegenerator.h"

tfecore::GameBoard::GameBoard() {
    for (int i = 0; i < 2;) {
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
            bool movePossible = false;
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++) {
                    if (board[y][x] <= 0) {
                        movePossible = true;
                        break;
                    }
                }

                if (movePossible) {
                    break;
                }
            }

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

void tfecore::GameBoard::collapse(MoveDirection direction) {
    tfecore::CollapseStrategy cs;
    switch (direction) {
        case tfecore::MoveDirection::DOWN:
            cs.collapseDown(board);
            break;
        case tfecore::MoveDirection::LEFT:
            cs.collapseLeft(board);
        break;
        case tfecore::MoveDirection::RIGHT:
            cs.collapseRight(board);
            break;
        case tfecore::MoveDirection::UP:
            cs.collapseUp(board);
            break;
        default:
            std::cout << "Not yet implemented" << std::endl;
            break;
    }
}

void tfecore::GameBoard::print() {
    std::cout << " +-------+-------+-------+-------+" << std::endl;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int val = board[y][x] > 0 ? (1 << board[y][x]) : 0;
            std::cout << " | " << std::setw(5) << val;
        }

        std::cout << " |" << std::endl;
        std::cout << " +-------+-------+-------+-------+" << std::endl;
    }

    std::cout << std:: endl;
}
