#pragma once

#include <queue>

namespace tfecore {
    class CollapseStrategy {
    private:
        bool collapseHorizontal(std::queue<std::tuple<int, int>> tileQueue, int collapseDirection, int board[][4]);

        bool collapseVertical(std::queue<std::tuple<int, int>> tileQueue, int collapseDirection, int board[][4]);

    public:
        bool collapseDown(int board[][4]);

        bool collapseLeft(int board[][4]);

        bool collapseRight(int board[][4]);

        bool collapseUp(int board[][4]);
    };
}
