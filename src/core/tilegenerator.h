#pragma once

namespace tfecore {
    class TileGenerator {
    public:
        TileGenerator();

        std::tuple<int, int, int> next();
    };
}
