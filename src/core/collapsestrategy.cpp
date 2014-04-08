#include <iostream>
#include <vector>

class TileOperation {
public:
    virtual void execute(std::vector<std::vector<int>> &board) = 0;
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

    virtual void execute(std::vector<std::vector<int>> &board) override {
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

    virtual void execute(std::vector<std::vector<int>> &board) override {
        int value = board[_yTarget][_xTarget];
        // remove current tile
        board[_yTarget][_xTarget] = 0;

        // slide
        board[_yDest][_xDest] = value;
    }
};


#include <queue>
#include "collapsestrategy.h"

bool tfecore::CollapseStrategy::collapseHorizontal(std::queue<std::tuple<int, int>> tileQueue, int collapseDirection, std::vector<std::vector<int>> &board) {
    size_t xCollide = collapseDirection > 0 ? board[0].size() - 1 : 0;

    bool collapsed = false;
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

bool tfecore::CollapseStrategy::collapseVertical(std::queue<std::tuple<int, int>> tileQueue, int collapseDirection, std::vector<std::vector<int>> &board) {
    int yCollide = collapseDirection > 0 ? board.size() - 1 : 0;

    bool collapsed = false;
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
        else if (board[yCollide - collapseDirection][x] <= 0) {
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

bool tfecore::CollapseStrategy::collapseDown(std::vector<std::vector<int>> &board) {
    bool collapsed = false;
    for (size_t x = 0; x < board[0].size(); x++) {

        std::queue<std::tuple<int, int>> tileQueue;
        for (size_t y = board.size() - 1; y > 0; y--) { // -1 to skip edge
            tileQueue.push(std::tuple<int, int>(x, y - 1)); // -1 for index
        }

        collapsed |= collapseVertical(tileQueue, 1, board);
    }

    return collapsed;
}

bool tfecore::CollapseStrategy::collapseLeft(std::vector<std::vector<int>> &board) {
    bool collapsed = false;
    for (size_t y = 0; y < board.size(); y++) {

        std::queue<std::tuple<int, int>> tileQueue;
        for (size_t x = 1; x < board[y].size(); x++) {
            tileQueue.push(std::tuple<int, int>(x, y));
        }

        collapsed |= collapseHorizontal(tileQueue, -1, board);
    }

    return collapsed;
}

bool tfecore::CollapseStrategy::collapseRight(std::vector<std::vector<int>> &board) {
    bool collapsed = false;
    for (size_t y = 0; y < board.size(); y++) {

        std::queue<std::tuple<int, int>> tileQueue;
        for (size_t x = board[y].size() - 1; x > 0; x--) { // -1 to skip edge
            tileQueue.push(std::tuple<int, int>(x - 1, y)); // -1 for index
        }

        collapsed |= collapseHorizontal(tileQueue, 1, board);
    }

    return collapsed;
}

bool tfecore::CollapseStrategy::collapseUp(std::vector<std::vector<int>> &board) {
    bool collapsed = false;
    for (size_t x = 0; x < board[0].size(); x++) {

        std::queue<std::tuple<int, int>> tileQueue;
        for (size_t y = 1; y < board.size(); y++) {
            tileQueue.push(std::tuple<int, int>(x, y));
        }

        collapsed |= collapseVertical(tileQueue, -1, board);
    }

    return collapsed;
}
