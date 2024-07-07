#pragma once

namespace eng::mesh {

struct Vertex
{
    float x, y, z;
    float norm_x, norm_y, norm_z;
    float u, v;
};

struct WeirdVertex
{
    float u, v;
    float norm_x, norm_y, norm_z;
    float x, y, z;
};

}