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
    
    Face(const std::vector<std::size_t> &indices, int color)
        :_indices(indices), _color(color)
    {}
    
    int color() const { return _color; };
    
    std::size_t numIndices() const { return _indices.size(); }

    std::size_t &operator[](std::size_t index) { return _indices[index]; };
    const std::size_t &operator[](std::size_t index) const { return _indices[index]; };

private:
    
    std::vector<std::size_t> _indices;
    int _color;

};


class Object
{

public:
    
    Object(const std::vector<Vec4> &vertices, const std::vector<Face> &faces)
        :_vertices(vertices), _faces(faces)
    {}
    
    std::size_t numVertices() const { return _vertices.size(); }
    std::size_t numFaces() const { return _faces.size(); }
    
    Vec4 &vertexAt(std::size_t index) { return _vertices[index]; };
    const Vec4 &vertexAt(std::size_t index) const { return _vertices[index]; };

    Face &faceAt(std::size_t index) { return _faces[index]; };
    const Face &faceAt(std::size_t index) const { return _faces[index]; };

private:
    
    std::vector<Vec4> _vertices;
    std::vector<Face> _faces;

};


#endif  // OBJECT_H