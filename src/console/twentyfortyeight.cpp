#include <iostream>
#include <ncurses.h>
#include "gameboard.h"

class ConsoleMoveDirectionReader {

public:
    tfecore::MoveDirection next() {

        while (true) {
            printw("WASD then <enter> to move: ");
            int keyCode = getch();

            // ignore subsequent characters so next read is fresh
            //std::cin.clear();
            //std::cin.ignore(INT_MAX,'\n');

            if (keyCode == 97) {
                return tfecore::MoveDirection::LEFT;
            }
            else if (keyCode == 115) {
                return tfecore::MoveDirection::DOWN;
            }
            else if (keyCode == 100) {
                return tfecore::MoveDirection::RIGHT;
            }
            else if (keyCode == 119) {
                return tfecore::MoveDirection::UP;
            }
        }
    }
};

#include <stdlib.h>

int main(int argc, char** argv) {
    initscr();
    cbreak(); // don't wait for line break
    noecho(); // don't show user's keypresses

    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    printw("[ TwentyFortyEight ]");
    attroff(COLOR_PAIR(1));
    refresh();
    getch();

    size_t boardSize = 4;
    size_t startingTiles = 2;

    if (argc >= 2) {
        boardSize = std::atoi(argv[1]);
        boardSize = boardSize > 1 ? boardSize : 4;
    }

    if (argc >= 3) {
        startingTiles = std::atoi(argv[2]);
        startingTiles = startingTiles > 0 && startingTiles <= boardSize*boardSize ? startingTiles : 2;
    }

    tfecore::GameBoard board = tfecore::GameBoard(boardSize, startingTiles);

    ConsoleMoveDirectionReader directionReader;

    while (true) {
        board.print();
        tfecore::MoveDirection direction = directionReader.next();
        if (board.collapse(direction)) {
            board.addTile();
        }
        else
        {
            if (!board.canCollapse()) {
                board.print();
                printw("GAME OVER!");
                break;
            }
        }
    }

    getch();
    endwin();

    return 0;
}
