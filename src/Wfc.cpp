#include <string>
#include <sstream>
#include <cassert>
#include "Wfc.h"
#include "Grid3D.h"
//#include "WfcTexture.h"
#include <random>
#include <stdlib.h>
#include <algorithm>
#include <optional>
WFC::WFC(int rows, int cols, int depth, TileSet image, unsigned int seed) : grid(rows, cols, depth), rng(std::random_device{}())
{

    unsigned int randomSeed = std::random_device{}();
    rng.seed(randomSeed);

    createEdges(image);
    //createCompatibilitys();
    addPossibleTiles();
    //printBoard();
    // printEdges();

     //printTiles();
    // return;
    while (true)
    {
        ObserveStatus status = observe();

        if (status == Success)
        {
            std::cout << "Success\n";
            break;
        }

        AC3();
        //printBoard();

        //printTiles();

             //break;
    }

    printBoard();
}

ObserveStatus WFC::observe()
{
    
    std::tuple<std::optional<Wave*>, int> pos = getNextPos();

    auto [waveOpt, idx] = pos;

    if (!waveOpt)
    {
        return Success;
    }

    Wave* wave = *waveOpt;
    getRandomTile(wave, m_Seed);
    std::cout << "escolhi: " << wave->x << wave->y << wave->z << "\n";

    if (queue.empty())
    {
        queue.push(wave);
    }




    return Continue;
}

void WFC::getRandomTile(Wave* wave, unsigned int seed)
{

    auto& domain = wave->possibleTiles;

    std::uniform_int_distribution<int> dist(0, domain.size() - 1);
    int idx = dist(rng);

    int chosenId = domain[idx];

    domain.clear();
    domain.push_back(chosenId);

    wave->tileID = chosenId;
    //std::cout << "escolhi o tile " << grid.graph[x][y].tileID << " na posicao " << x << "," << y << "\n";
    //std::cout << "Random: " << random << " PossibleTiles: " << grid.graph[x][y].tile.possibleTiles.size() << " Tile: " << id<< "\n";


    // std::cout << "Tile " << grid.graph[x][y].tile.id << ": ";

    // std::cout << "Compatible UP: ";
    // for(int j = 0; j <  grid.graph[x][y].tile.compatible[UP].size(); j++)
    // {
    //     std::cout <<   grid.graph[x][y].tile.compatible[UP][j] << " ";
    // }

    // std::cout << "Compatible DOWN: ";
    // for(int j = 0; j <   grid.graph[x][y].tile.compatible[DOWN].size(); j++)
    // {
    //     std::cout <<   grid.graph[x][y].tile.compatible[DOWN][j] << " ";
    // }

    // std::cout << "Compatible RIGHT: ";
    // for(int j = 0; j <   grid.graph[x][y].tile.compatible[RIGHT].size(); j++)
    // {
    //     std::cout <<   grid.graph[x][y].tile.compatible[RIGHT][j] << " ";
    // }

    // std::cout << "Compatible LEFT: ";
    // for(int j = 0; j < grid.graph[x][y].tile.compatible[LEFT].size(); j++)
    // {
    //     std::cout <<  grid.graph[x][y].tile.compatible[LEFT][j] << " ";
    // }
    // std::cout << "\n";

    // std::cout << random << "\n" << "tile size:" << tiles.size() << "\n";
}


bool WFC::AC3()
{
    const int dx[] = { 1,-1, 0, 0, 0, 0 };
    const int dy[] = { 0, 0, 1,-1, 0, 0 };
    const int dz[] = { 0, 0, 0, 0, 1,-1 };

    while (!queue.empty())
    {
        std::cout << "ac3\n";
        Wave* wave = queue.front();
        queue.pop();


        for (int i = 0; i < 6; i++)
        {
            std::cout << "i: " << i << "\n";
            int cx = wave->x + dx[i];
            int cy = wave->y + dy[i];
            int cz = wave->z + dz[i];
            std::cout << "nei: " << cx << cy << cz << "\n";
            std::cout << "pai" << wave->x << wave->y << wave->z << "\n";

            Wave* child = grid.tryAt(cx,cy,cz);
            if (child == nullptr) continue;

            bool changed = restraintPropagation(wave, child);

            if (child->possibleTiles.empty())
            {
                std::cout << "Contradicao em " << cx << "," << cy << "," << cz << "\n";
                return false;
            }


            if (changed)
                queue.push(child);
            std::cout << "empilheei child\n";

        }


    }
    return true;
}

bool WFC::isCompatible(int fatherTileId, int childTileId, int direction)
{
    std::cout << "Compatible\n";
    int childDir = opossiteDirection(direction);

    auto& fatherCompat = tiles[fatherTileId].compatible[direction];

    bool fatherAcceptsChild = std::find(fatherCompat.begin(), fatherCompat.end(), childTileId) != fatherCompat.end();

    // Pega os compatíveis do filho na direção oposta
    auto& childCompat = tiles[childTileId].compatible[childDir];

    // Verifica se o ID do pai está na lista do filho
    bool childAcceptsFather = std::find(childCompat.begin(), childCompat.end(), fatherTileId) != childCompat.end();



    return fatherAcceptsChild && childAcceptsFather;
}

bool WFC::restraintPropagation(Wave* father, Wave* child)
{
    std::cout << "restraint propagation\n";
    int direction = checkDirection(*father, *child);
    std::vector<int> remove;
    std::cout << "check direction\n";
    std::cout << "father: " << father->x << father->y << father->z << "Child: " << child->x << child->y << child->z << "\n";
    for (int childTileId : child->possibleTiles)
    {
        std::cout << "possible\n";

        bool hasSupport = false;

        for (int fatherTileId : father->possibleTiles)
        {
            if (isCompatible(fatherTileId, childTileId, direction))
            {
                hasSupport = true;
                break;
            }
        }

        if (!hasSupport)
        {
            // std::cout  << " pai " << father->row << "," << father->col << " filho " << child->row << "," << child->col << " Removendo: " << childTileId << "\n";
            remove.push_back(childTileId);

        }
    }

    if (remove.empty())
        return false;

    for (int id : remove)
    {
        auto it = std::find(
            child->possibleTiles.begin(),
            child->possibleTiles.end(),
            id
        );
        if (it != child->possibleTiles.end())
            child->possibleTiles.erase(it);
    }

    return true;
}


Tile WFC::getTile(int tileID)
{
    return tiles[tileID];
}

void WFC::createEdges(TileSet images)
{
    for (const auto& text : images.textures)
    {
        Tile tile;
        std::stringstream ss(text->m_Name);
        std::string token;

        // Lê o ID
        std::getline(ss, token, '_');
        tile.id = stoi(token);

        // Lê as 6 direções: UP, DOWN, LEFT, RIGHT, FRONT, BACK
        int dir = 0;
        while (std::getline(ss, token, '_') && dir < 6)
        {
            for (char c : token)
            {
                tile.compatible[dir].push_back(c - '0');
            }
            dir++;
        }

        tiles.push_back(tile);

    }
}

void WFC::createCompatibilitys()
{
    for (Tile& a : tiles)
    {
        for (Tile& b : tiles)
        {
            if (a.edge[UP] == b.edge[DOWN])
                a.compatible[UP].push_back(b.id);

            if (a.edge[DOWN] == b.edge[UP])
                a.compatible[DOWN].push_back(b.id);

            if (a.edge[RIGHT] == b.edge[LEFT])
                a.compatible[RIGHT].push_back(b.id);

            if (a.edge[LEFT] == b.edge[RIGHT])
                a.compatible[LEFT].push_back(b.id);
        }
    }
}

void WFC::addPossibleTiles()
{
    for (int i = 0; i < grid.width(); i++)
    {
        for (int j = 0; j < grid.height(); j++)
        {
            for (int k = 0; k < grid.depth(); k++)
            {
                Wave* wave = grid.tryAt(i, j, k);
                wave->x = i;
                wave->y = j;
                wave->z = k;
                for (int x = 0; x < tiles.size(); ++x)
                    wave->possibleTiles.push_back(x);
            }
        }
    }
}

std::tuple<std::optional<Wave*>, int> WFC::getNextPos()
{
    for (int i = 0; i < grid.width(); i++)
    {
        for (int j = 0; j < grid.height(); j++)
        {
            for (int k = 0; k < grid.depth(); k++)
            {
                Wave* wave = grid.tryAt(i, j, k);
                if (wave->tileID == UNCOLLAPSED)
                {
                    return std::make_tuple(wave, Continue);

                }
            }
        }
    }

    return std::make_tuple(std::nullopt, Success);
}

Direction WFC::checkDirection(Wave father, Wave child)
{

    int dx = child.x - father.x;
    int dy = child.y - father.y;
    int dz = child.z - father.z;

    if (dx == 1) return RIGHT;
    if (dx == -1) return LEFT;
    if (dy == 1) return UP;
    if (dy == -1) return DOWN;
    if (dz == 1) return FRONT;
    if (dz == -1) return BACK;
}

int WFC::opossiteDirection(int direction)
{
    switch (direction)
    {
    case UP:
        return DOWN;
    case DOWN:
        return UP;
    case RIGHT:
        return LEFT;
    case LEFT:
        return RIGHT;
    case FRONT:
        return BACK;
    case BACK:
        return FRONT;
    default:
        return -1;
    }
}

void WFC::printTiles()
{

    for (int i = 0; i < tiles.size(); i++)
    {
        std::cout << "Tile ID: " << tiles[i].id << "\n";
        for (int j = 0; j < tiles[i].compatible[UP].size(); j++)
        {
            std::cout << "UP:" << tiles[i].compatible[UP][j] << "\n";
        }

        for (int j = 0; j < tiles[i].compatible[DOWN].size(); j++)
        {
            std::cout << "DOWN:" << tiles[i].compatible[DOWN][j] << "\n";
        }

        for (int j = 0; j < tiles[i].compatible[LEFT].size(); j++)
        {
            std::cout << "LEFT:" << tiles[i].compatible[LEFT][j] << "\n";
        }

        for (int j = 0; j < tiles[i].compatible[RIGHT].size(); j++)
        {
            std::cout << "RIGHT:" << tiles[i].compatible[RIGHT][j] << "\n";
        }

        for (int j = 0; j < tiles[i].compatible[FRONT].size(); j++)
        {
            std::cout << "FRONT:" << tiles[i].compatible[FRONT][j] << "\n";
        }

        for (int j = 0; j < tiles[i].compatible[BACK].size(); j++)
        {
            std::cout << "BACK:" << tiles[i].compatible[BACK][j] << "\n";
        }
    }
}

void WFC::printBoard()
{
    for (int i = 0; i < grid.width(); i++)
    {
        for (int j = 0; j < grid.height(); j++)
        {
            for (int k = 0; k < grid.depth(); k++)
            {
                Wave wave = grid.at(i, j, k);
                std::cout << "i:" << i << " j:" << j << " k:" << k << "\n";

                for (int x = 0; x < wave.possibleTiles.size(); x++)
                {
                    std::cout << x;
                }
                std::cout << "\n";
            }
        }
    }
}

void WFC::printEdges()
{
    for (auto& tile : tiles)
    {
 /*       std::cout << "Tile " << tile.id << ": ";
        std::cout << "Edge UP: " << tile.edge[UP] << " ";
        std::cout << "Edge DOWN: " << tile.edge[DOWN] << " ";
        std::cout << "Edge RIGHT: " << tile.edge[RIGHT] << " ";
        std::cout << "Edge LEFT: " << tile.edge[LEFT] << " ";
        std::cout << "\n";*/
    }
}