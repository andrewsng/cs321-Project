#include "polygon.h"

#include <iostream>
using std::cerr;


inline void indexForward(int &index, int length)
{
    index = (index + 1) % length;
}


inline void indexBackward(int &index, int length)
{
    index = (index - 1 + length) % length;
}


inline void indexMove(int &index, int length, int direction)
{
    if (direction > 0)
        indexForward(index, length);
    else
        indexBackward(index, length);
}


bool fillConvexPolygon(const PointList &vertexList, int color, int xOffset, int yOffset)
{
    if (vertexList.length == 0)
        return false;

    const Point *vertices = vertexList.points;
    
    int minIndexL = 0, maxIndex = 0;
    int minPointY = vertices[minIndexL].y;
    int maxPointY = vertices[maxIndex].y;
    for (int i = 1; i < vertexList.length; ++i)
    {
        if (vertices[i].y < minPointY)
        {
            minIndexL = i;
            minPointY = vertices[minIndexL].y;
        }
        else if (vertices[i].y > maxPointY)
        {
            maxIndex = i;
            maxPointY = vertices[maxIndex].y;
        }
    }
    if (minPointY == maxPointY)
        return false;
    
    int minIndexR = minIndexL;
    while (vertices[minIndexR].y == maxPointY)
    {
        indexForward(minIndexR, vertexList.length);
    }
    indexBackward(minIndexR, vertexList.length);

    while (vertices[minIndexL].y == maxPointY)
    {
        indexBackward(minIndexR, vertexList.length);
    }
    indexForward(minIndexR, vertexList.length);
    
    cerr << "minPointY: " << minPointY << '\n';
    cerr << "maxPointY: " << maxPointY << '\n';
    cerr << "minIndexL: " << minIndexL << '\n';
    cerr << "minIndexR: " << minIndexR << '\n';
    cerr << "maxIndex: " << maxIndex << '\n';
}