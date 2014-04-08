#pragma once

namespace tfecore {
    class TileGenerator {
    private:
        size_t _boardSize;

    public:
        TileGenerator(size_t boardSize);

        std::tuple<int, int, int> next();
    };
}
