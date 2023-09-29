#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "Basic/Vector.hpp"
namespace ycad
{


struct Vertex
{
    Vector3f position;
    Vector2f tex_coords;
    Vector3f normal;
    Vector4f color;
    // float tex_index;
};

} //namespace ycad

#endif /*VERTEX_HPP*/