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


#include <queue>

class CollapseStrategy {
private:
    void collapseHorizontal(std::queue<std::tuple<int, int>> tileQueue, int collapseDirection, int board[][4]) {
        int xCollide = collapseDirection > 0 ? 3 : 0;

        std::tuple<int, int> lastCollidable = tileQueue.front();
        while (!tileQueue.empty()) {
            std::tuple<int, int> currentTile = tileQueue.front();
            tileQueue.pop();

            int x = std::get<0>(currentTile);
            int y = std::get<1>(currentTile);
            int value = board[y][x];

            // nothing to do here
            if (value <= 0) {
                continue;
            }

            // if can collapse
            if (value == board[y][xCollide]) {
                MergeTileOperation tileOperation = MergeTileOperation(xCollide, y, x, y);
                tileOperation.execute(board);

                // next collision is with the next tile
                xCollide = xCollide - collapseDirection;
            }

            // if can slide
            else if (board[y][xCollide] <= 0) {
                //board[y][xCollide] = value;
                MoveTileOperation tileOperation = MoveTileOperation(xCollide, y, x, y);
                tileOperation.execute(board);
            }
            // if can slide next to
            else if (board[y][xCollide - collapseDirection] <= 0) {
                // next collision is with this tile
                xCollide = xCollide - collapseDirection;

                MoveTileOperation tileOperation = MoveTileOperation(xCollide, y, x, y);
                tileOperation.execute(board);
            }
            else {
                // next collision is with this tile
                xCollide = x;
            }
        }
    }
public:
    void collapseLeft(int board[][4]) {
        for (int y = 0; y < 4; y++) {

            std::queue<std::tuple<int, int>> tileQueue;
            for (int x = 1; x < 4; x++) {
                tileQueue.push(std::tuple<int, int>(x, y));
            }

            collapseHorizontal(tileQueue, -1, board);
        }
    }

    void collapseRight(int board[][4]) {
        for (int y = 0; y < 4; y++) {

            std::queue<std::tuple<int, int>> tileQueue;
            for (int x = 4 - 2; x >= 0; x--) {
                tileQueue.push(std::tuple<int, int>(x, y));
            }

            collapseHorizontal(tileQueue, 1, board);
        }
    }
};

#include <cstdlib>

class GameBoard {
private:
    TileGenerator tileGenerator;
    int board[4][4] = {{ 0 }}; // initialize board to 0

public:
    GameBoard() {
        for (int i = 0; i < 2;) {
            if(addTile()) {
                i++;
            }
        }
    }

    bool addTile() {
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

    void collapse(MoveDirection direction) {
        CollapseStrategy cs;
        switch (direction) {
            case MoveDirection::LEFT:
                cs.collapseLeft(board);
            break;
            case MoveDirection::RIGHT:
                cs.collapseRight(board);
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
        if (!board.addTile()) {
            std::cout << "YOU LOSE! GAME OVER!" << std::endl;
            break;
        }
    }

    return 0;
}
