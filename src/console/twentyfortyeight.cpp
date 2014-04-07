#include <iostream>
#include "gameboard.h"


class ConsoleMoveDirectionReader {

public:
    tfecore::MoveDirection next() {

        while (true) {
            std::cout << "WASD then <enter> to move: ";
            int keyCode = std::cin.get();

            // ignore subsequent characters so next read is fresh
            std::cin.clear();
            std::cin.ignore(INT_MAX,'\n');

            if (keyCode == 97) {
                return tfecore::MoveDirection::LEFT;
                break;
            }
            else if (keyCode == 115) {
                return tfecore::MoveDirection::DOWN;
                break;
            }
            else if (keyCode == 100) {
                return tfecore::MoveDirection::RIGHT;
                break;
            }
            else if (keyCode == 119) {
                return tfecore::MoveDirection::UP;
                break;
            }
        }
    }
};



int main() {
    std::cout << "[ TwentyFortyEight ]" << std::endl;

    tfecore::GameBoard board;

    ConsoleMoveDirectionReader directionReader;

    while (true) {
        board.print();
        tfecore::MoveDirection direction = directionReader.next();
        if (board.collapse(direction)) {
            if (!board.addTile()) {
                std::cout << "YOU LOSE! GAME OVER!" << std::endl;
                break;
            }
        }
    }

    return 0;
}
