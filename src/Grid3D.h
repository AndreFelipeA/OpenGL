#pragma once
#include <vector>
#include "Wave.h"
#include <cassert>

class Grid3D {
private:
    int X, Y, Z;
    std::vector<Wave> data;

    inline size_t index(int x, int y, int z) const {
        return x + X * (y + Y * z);
    }

    inline bool inBounds(int x, int y, int z) const {
        return x >= 0 && x < X &&
            y >= 0 && y < Y &&
            z >= 0 && z < Z;
    }

public:
    Grid3D(int x, int y, int z)
        : X(x), Y(y), Z(z),
        data(x* y* z) {
    }

    Wave& at(int x, int y, int z) {
        assert(inBounds(x, y, z));
        return data[index(x, y, z)];
    }

    Wave* tryAt(int x, int y, int z)
    {
        if (!inBounds(x, y, z))
            return nullptr;

        return &data[index(x, y, z)];
    }

    const Wave& at(int x, int y, int z) const {
        assert(inBounds(x, y, z));
        return data[index(x, y, z)];
    }

    int width()  const { return X; }
    int height() const { return Y; }
    int depth()  const { return Z; }
};