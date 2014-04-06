#include <iostream>

enum MoveDirection { LEFT, UP, RIGHT, DOWN };

class ConsoleMoveDirectionReader {

public:
    MoveDirection next() {

        while (true) {
            std::cout << "WASD then <enter> to move: ";
            int keyCode = std::cin.get();
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

#include <cstdlib>

void print (int board[4][4]) {
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

int main() {
    std::cout << "[ TwentyFortyEight ]" << std::endl;

    int board[4][4];
    TileGenerator tileGenerator;
    for (int i = 0; i < 2; i++) {
        std::tuple<int, int, int> tile = tileGenerator.next();
        int xPos = std::get<0>(tile);
        int yPos = std::get<1>(tile);
        board[yPos][xPos] = std::get<2>(tile);
    }

    ConsoleMoveDirectionReader directionReader;

    while (true) {
        print(board);
        MoveDirection direction = directionReader.next();
        std::cout << (int)direction << std::endl;
    }

    return 0;
}
