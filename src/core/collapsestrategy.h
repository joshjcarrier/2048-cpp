#pragma once

#include <queue>
#include <vector>

namespace tfecore {
    class CollapseStrategy {
    private:
        bool collapseHorizontal(std::queue<std::tuple<int, int>> tileQueue, int collapseDirection, std::vector<std::vector<int>> &board);

        bool collapseVertical(std::queue<std::tuple<int, int>> tileQueue, int collapseDirection, std::vector<std::vector<int>> &board);

    public:
        bool collapseDown(std::vector<std::vector<int>> &board);

        bool collapseLeft(std::vector<std::vector<int>> &board);

        bool collapseRight(std::vector<std::vector<int>> &board);

        bool collapseUp(std::vector<std::vector<int>> &board);
    };
}
