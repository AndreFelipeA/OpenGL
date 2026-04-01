#include "PerlinNoise.h"
#include <algorithm>
#include <random>

PerlinNoise::PerlinNoise(unsigned int seed)
{
    std::vector<int> p;
    for(int i = 0; i < 256; i++)
    {
        p.push_back(i);
    }
    std::mt19937 rng;
    rng.seed(seed);
    std::shuffle(p.begin(), p.end(), rng);

    for(int i = 0; i < 256; i++)
    {
        m_LookUp.push_back(p[i]);
    }

    for(int i = 0; i < 256; i++)
    {
        m_LookUp.push_back(p[i]);
    }

}

float PerlinNoise::PerlinNoise3D(float x, float y, float z)
{
    int X,Y,Z;
    X = (int)std::floor(x) & 255;
    Y = (int)std::floor(y) & 255;
    Z = (int)std::floor(z) & 255;

    float xf = x - std::floor(x);
    float yf = y - std::floor(y);
    float zf = z - std::floor(z);

    float u = Fade(xf);
    float v = Fade(yf);
    float w = Fade(zf);

    int A = m_LookUp[X] + Y;
    int B = m_LookUp[X+1] + Y;

    int aaa = m_LookUp[m_LookUp[A] + Z];
    int aba = m_LookUp[m_LookUp[A+1] + Z];
    int aab = m_LookUp[m_LookUp[A] + Z+1];
    int abb = m_LookUp[m_LookUp[A+1] +Z+1 ];
    int baa = m_LookUp[m_LookUp[B] +Z];
    int bba = m_LookUp[m_LookUp[B+1] +Z];
    int bab = m_LookUp[m_LookUp[B] +Z+1];
    int bbb = m_LookUp[m_LookUp[B+1] +Z+1];

    float gradA_x1 = Grad3D(aaa, xf, yf, zf);
    float gradB_x1 = Grad3D(baa, xf-1, yf, zf);
    float x1 = Lerp(gradA_x1, gradB_x1, u);

    float gradA_x2 = Grad3D(aba, xf, yf-1, zf);
    float gradB_x2 = Grad3D(bba, xf-1, yf-1, zf);
    float x2 = Lerp(gradA_x2, gradB_x2, u);


    float frente = Lerp(x1, x2, v);

    float gradA_x3 = Grad3D(aab, xf, yf, zf-1);
    float gradB_x3 = Grad3D(bab, xf-1, yf, zf-1);
    float x3 = Lerp(gradA_x3, gradB_x3, u);

    float gradA_x4 = Grad3D(abb, xf, yf-1, zf-1);
    float gradB_x4 = Grad3D(bbb, xf-1, yf-1, zf-1);
    float x4 = Lerp(gradA_x4, gradB_x4, u);

    float fundo = Lerp(x3, x4, v);

    return Lerp(frente, fundo, w);


}

float PerlinNoise::PerlinOctaves3D(float x, float y, float z, int octaves, float persistence)
{
    float value = 0;
    float amplitude = 1;
    float freq = 1;
    float max_value = 0;

    for(int i = 0; i < octaves; i++)
    {
        value += PerlinNoise3D(x * freq, y * freq, z * freq) * amplitude;
        max_value += amplitude;
        amplitude *= persistence;
        freq *= 2;
    }

    return value/max_value;
}

float PerlinNoise::Fade(float t)
{
    return t * t * t * (t * (t * 6- 15) +10);
}

float PerlinNoise::Lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

float PerlinNoise::Grad3D(int hash, float x, float y, float z)
{
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : (h == 12 || h == 14) ? x : z;
    return ((h & 1) == 0 ? u  : -u) + ((h & 2) == 0 ? v : -v);
}
