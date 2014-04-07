#include <iostream>

enum MoveDirection { LEFT, UP, RIGHT, DOWN };

class ConsoleMoveDirectionReader {

public:
    MoveDirection next() {

        while (true) {
            std::cout << "WASD then <enter> to move: ";
            int keyCode = std::cin.get();

            // ignore subsequent characters so next read is fresh
            std::cin.clear();
            std::cin.ignore(INT_MAX,'\n');

            if (keyCode == 97) {
                return MoveDirection::LEFT;
                break;
            }
            else if (keyCode == 115) {
                return MoveDirection::DOWN;
                break;
            }
            else if (keyCode == 100) {
                return MoveDirection::RIGHT;
                break;
            }
            else if (keyCode == 119) {
                return MoveDirection::UP;
                break;
            }
        }
    }
};

class TileGenerator {
public:
    TileGenerator() {
        std::srand(0);
    }

    std::tuple<int, int, int> next() {
        int value = std::rand() % 10 == 0 ? 2 : 1;
        return std::tuple<int, int, int>(std::rand() % 4, std::rand() % 4, value);
    }
};

class TileOperation {
public:
    virtual void execute(int board[][4]) = 0;
};

class MergeTileOperation : public TileOperation {
    int _xDest, _yDest, _xTarget, _yTarget;

public:
    MergeTileOperation(int xDest, int yDest, int xTarget, int yTarget) {
        _xDest = xDest;
        _yDest = yDest;
        _xTarget = xTarget;
        _yTarget = yTarget;
    }

    virtual void execute(int board[][4]) override {
        // remove current tile
        board[_yTarget][_xTarget] = 0;

        // merge
        board[_yDest][_xDest] += 1;
    }
};

class MoveTileOperation : public TileOperation {
    int _xDest, _yDest, _xTarget, _yTarget;

public:
    MoveTileOperation(int xDest, int yDest, int xTarget, int yTarget) {
        _xDest = xDest;
        _yDest = yDest;
        _xTarget = xTarget;
        _yTarget = yTarget;
    }

    virtual void execute(int board[][4]) override {
        int value = board[_yTarget][_xTarget];
        // remove current tile
        board[_yTarget][_xTarget] = 0;

        // slide
        board[_yDest][_xDest] = value;
    }
};

#include <cstdlib>
#include <stack>

class GameBoard {
private:
    TileGenerator tileGenerator;
    int board[4][4] = {{ 0 }}; // initialize board to 0

public:
    GameBoard() {
        for (int i = 0; i < 2; i++) {
            addTile();
        }
    }

    bool addTile() {
        std::tuple<int, int, int> tile;
        while (true) {
            tile = tileGenerator.next();
            int xPos = std::get<0>(tile);
            int yPos = std::get<1>(tile);

            if (board[yPos][xPos] > 0) {
                // TODO check if any spot is possible
                //return false;
                continue;
            }

            break;
        }

        int xPos = std::get<0>(tile);
        int yPos = std::get<1>(tile);
        board[yPos][xPos] = std::get<2>(tile);
        return true;
    }

    void collapse(MoveDirection direction) {
        switch (direction) {
            case MoveDirection::LEFT:
                for (int y = 0; y < 4; y++) {
                    // scans from right to left
                    int xCollide = 0;
                    for (int x = 1; x < 4; x++) {

                    int value = board[y][x];

                    // nothing to do here
                    if (value <= 0) {
                        continue;
                    }

                    // if can collapse
                    if (value == board[y][xCollide]) {
                        MergeTileOperation tileOperation = MergeTileOperation(xCollide, y, x, y);
                        tileOperation.execute(board);
                    }

                    // if can slide
                    else if (board[y][xCollide] <= 0) {
                        //board[y][xCollide] = value;
                        MoveTileOperation tileOperation = MoveTileOperation(xCollide, y, x, y);
                        tileOperation.execute(board);
                    }
                    // if can slide next to
                    else if (board[y][xCollide + 1] <= 0) {
                        // next collision is with this tile
                        xCollide = xCollide + 1;

                        MoveTileOperation tileOperation = MoveTileOperation(xCollide, y, x, y);
                        tileOperation.execute(board);
                    }
                    else {
                        // next collision is with this tile
                        xCollide = x;
                    }
                }
            }
            break;
            case MoveDirection::RIGHT:
                for (int y = 0; y < 4; y++) {
                    // scans from right to left
                    int xCollide = 3;
                    for (int x = 4 - 2; x >= 0; x--) {

                        int value = board[y][x];

                        // nothing to do here
                        if (value <= 0) {
                            continue;
                        }

                        // if can collapse
                        if (value == board[y][xCollide]) {
                            MergeTileOperation tileOperation = MergeTileOperation(xCollide, y, x, y);
                            tileOperation.execute(board);
                        }

                        // if can slide
                        else if (board[y][xCollide] <= 0) {
                            //board[y][xCollide] = value;
                            MoveTileOperation tileOperation = MoveTileOperation(xCollide, y, x, y);
                            tileOperation.execute(board);
                        }
                        // if can slide next to
                        else if (board[y][xCollide - 1] <= 0) {
                            // next collision is with this tile
                            xCollide = xCollide - 1;

                            MoveTileOperation tileOperation = MoveTileOperation(xCollide, y, x, y);
                            tileOperation.execute(board);
                        }
                        else {
                            // next collision is with this tile
                            xCollide = x;
                        }
                    }
                }
                break;
            default:
                std::cout << "Not yet implemented" << std::endl;
                break;
        }
    }

    void print() {
        std::cout << " +---+---+---+---+" << std::endl;
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                int val = board[y][x] > 0 ? (1 << board[y][x]) : 0;
                std::cout << " | " << val;
            }

            std::cout << " |" << std::endl;
            std::cout << " +---+---+---+---+" << std::endl;
        }

        std::cout << std:: endl;
    }
};

int main() {
    std::cout << "[ TwentyFortyEight ]" << std::endl;

    GameBoard board = GameBoard();

    ConsoleMoveDirectionReader directionReader;

    while (true) {
        board.print();
        MoveDirection direction = directionReader.next();
        board.collapse(direction);
        board.addTile();
    }

    return 0;
}
