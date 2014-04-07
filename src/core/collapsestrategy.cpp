#include <iostream>

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

        // huh
        if (board[_yDest][_xDest] == 11) {
            std::cout << "~~~~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!~~~" << std::endl;
            std::cout << "~~~~! Congratulations on making 2048 !~~~" << std::endl;
            std::cout << "~~~~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!~~~" << std::endl;
        }
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
#include "collapsestrategy.h"

bool tfecore::CollapseStrategy::collapseHorizontal(std::queue<std::tuple<int, int>> tileQueue, int collapseDirection, int board[][4]) {
    int xCollide = collapseDirection > 0 ? 3 : 0;

    bool collapsed;
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
            collapsed = true;

            // next collision is with the next tile
            xCollide = xCollide - collapseDirection;
        }

        // if can slide
        else if (board[y][xCollide] <= 0) {
            MoveTileOperation tileOperation = MoveTileOperation(xCollide, y, x, y);
            tileOperation.execute(board);
            collapsed = true;
        }
        // if can slide next to
        else if (board[y][xCollide - collapseDirection] <= 0) {
            // next collision is with this tile
            xCollide = xCollide - collapseDirection;

            MoveTileOperation tileOperation = MoveTileOperation(xCollide, y, x, y);
            tileOperation.execute(board);
            collapsed = true;
        }
        else {
            // next collision is with this tile
            xCollide = x;
        }
    }

    return collapsed;
}

bool tfecore::CollapseStrategy::collapseVertical(std::queue<std::tuple<int, int>> tileQueue, int collapseDirection, int board[][4]) {
    int yCollide = collapseDirection > 0 ? 3 : 0;

    bool collapsed = false;
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
        if (value == board[yCollide][x]) {
            MergeTileOperation tileOperation = MergeTileOperation(x, yCollide, x, y);
            tileOperation.execute(board);
            collapsed = true;

            // next collision is with the next tile
            yCollide = yCollide - collapseDirection;
        }

        // if can slide
        else if (board[yCollide][x] <= 0) {
            MoveTileOperation tileOperation = MoveTileOperation(x, yCollide, x, y);
            tileOperation.execute(board);
            collapsed = true;
        }
        // if can slide next to
        else if (board[y - collapseDirection][x] <= 0) {
            // next collision is with this tile
            yCollide = yCollide - collapseDirection;

            MoveTileOperation tileOperation = MoveTileOperation(x, yCollide, x, y);
            tileOperation.execute(board);
            collapsed = true;
        }
        else {
            // next collision is with this tile
            yCollide = y;
        }
    }

    return collapsed;
}

bool tfecore::CollapseStrategy::collapseDown(int board[][4]) {
    bool collapsed = false;
    for (int x = 0; x < 4; x++) {

        std::queue<std::tuple<int, int>> tileQueue;
        for (int y = 4 - 2; y >= 0; y--) {
            tileQueue.push(std::tuple<int, int>(x, y));
        }

        collapsed |= collapseVertical(tileQueue, 1, board);
    }

    return collapsed;
}

bool tfecore::CollapseStrategy::collapseLeft(int board[][4]) {
    bool collapsed = false;
    for (int y = 0; y < 4; y++) {

        std::queue<std::tuple<int, int>> tileQueue;
        for (int x = 1; x < 4; x++) {
            tileQueue.push(std::tuple<int, int>(x, y));
        }

        collapsed |= collapseHorizontal(tileQueue, -1, board);
    }

    return collapsed;
}

bool tfecore::CollapseStrategy::collapseRight(int board[][4]) {
    bool collapsed = false;
    for (int y = 0; y < 4; y++) {

        std::queue<std::tuple<int, int>> tileQueue;
        for (int x = 4 - 2; x >= 0; x--) {
            tileQueue.push(std::tuple<int, int>(x, y));
        }

        collapsed |= collapseHorizontal(tileQueue, 1, board);
    }

    return collapsed;
}

bool tfecore::CollapseStrategy::collapseUp(int board[][4]) {
    bool collapsed = false;
    for (int x = 0; x < 4; x++) {

        std::queue<std::tuple<int, int>> tileQueue;
        for (int y = 1; y < 4; y++) {
            tileQueue.push(std::tuple<int, int>(x, y));
        }

        collapsed |= collapseVertical(tileQueue, -1, board);
    }

    return collapsed;
}
