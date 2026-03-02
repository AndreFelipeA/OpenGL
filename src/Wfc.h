#pragma once

#include <vector>
#include "Grid3D.h"
#include "TileSet.h"
#include <tuple>
#include <queue>
#include <random>
#include <optional>

enum ObserveStatus
{
    Success,
    Failure,
    Continue
};

class WFC {
public:
    std::mt19937 rng;
    Grid3D grid;
    std::vector<Tile> tiles;
    std::queue<Wave*> queue;

    WFC(int rows, int cols, int depth, TileSet image, unsigned int seed);

    void createEdges(TileSet images);

    void createCompatibilitys();

    void addPossibleTiles();

    void printTiles();

    Direction checkDirection(Wave father, Wave child);

    int opossiteDirection(int direction);

    std::tuple<std::optional<Wave*>, int> getNextPos();

    ObserveStatus observe();

    bool AC3();


    bool isCompatible(int fatherTileId, int childTileId, int direction);

    bool restraintPropagation(Wave* father, Wave* child);

    void getRandomTile(Wave* wave, unsigned int seed);

    Tile getTile(int tileID);

    void printBoard();

    void printEdges();

    Grid3D getGrid() { return grid; };

private:
    unsigned int m_Seed;

};
