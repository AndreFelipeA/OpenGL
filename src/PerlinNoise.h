#pragma once

#include <vector>


class PerlinNoise 
{
private:
    std::vector<int> m_LookUp;

    float Fade(float t);

    float Lerp(float a, float b, float t);
    

public:
    PerlinNoise(unsigned int seed);
    float PerlinNoise3D(float x, float y, float z);
    float Grad3D(int hash, float x, float y, float z);
    float PerlinOctaves3D(float x, float y, float z, int octaves, float persistence);
};