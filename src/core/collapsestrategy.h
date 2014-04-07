#pragma once

#include <queue>

namespace tfecore {
    class CollapseStrategy {
    private:
        void collapseHorizontal(std::queue<std::tuple<int, int>> tileQueue, int collapseDirection, int board[][4]);

        void collapseVertical(std::queue<std::tuple<int, int>> tileQueue, int collapseDirection, int board[][4]);

    public:
        void collapseDown(int board[][4]);

        void collapseLeft(int board[][4]);

        void collapseRight(int board[][4]);

        void collapseUp(int board[][4]);
    };
}
