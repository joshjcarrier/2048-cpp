#include <iostream>
//#include <stdio.h>

enum MoveDirection { LEFT, UP, RIGHT, DOWN };

class ConsoleMoveDirectionReader {

public:
    MoveDirection next() {

        while (true) {
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

int main() {
    std::cout << "hello world" << std::endl;

    ConsoleMoveDirectionReader directionReader;
    MoveDirection direction = directionReader.next();
    std::cout << (int)direction << std::endl;

    return 0;
}
