#pragma once

#include <vector>
#include <iostream>
#include <set>
#include <random>


enum Direction { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, FRONT = 4, BACK = 5 };

enum TileState { COLLAPSED = 0, UNCOLLAPSED = -1 };

struct Tile {
    int id;
    int edge[6];
    std::vector<int> compatible[6];
};

struct Wave {
    int x, y, z;
    int tileID = UNCOLLAPSED;
    std::vector<int> possibleTiles;

};


