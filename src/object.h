// object.h
// Andrew S. Ng
// Created: 2021-04-18
// 
// For CS 321 DOS VGA Graphics
// Header for 3D Object types

#ifndef OBJECT_H
#define OBJECT_H

#include "math3d.h"

#include <vector>


class Face
{

public:

private:
    
    std::vector<std::size_t> _indices;
    int _color;

};


class Object
{

public:
    
    std::size_t numVertices() const { return _vertices.size(); }
    
    Vec4 &vertexAt(std::size_t index) { return _vertices[index]; };
    const Vec4 &vertexAt(std::size_t index) const { return _vertices[index]; };

private:
    
    std::vector<Vec4> _vertices;
    std::vector<Face> _faces;

};


#endif  // OBJECT_H