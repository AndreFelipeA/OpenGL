#pragma once

struct Vec3 {
    float x,y,z;
};


struct Triangle {
    Vec3 p[3];
};

struct GridCell {
    Vec3 pos[8];
    double val[8];
};

