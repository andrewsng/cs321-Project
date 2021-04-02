#include "polygon.h"

#include <iostream>
using std::cerr;
#include <vector>
using std::vector;


inline int indexForward(int index, int length)
{
    return (index + 1) % length;
}


inline int indexBackward(int index, int length)
{
    return (index - 1 + length) % length;
}


inline int indexMove(int index, int length, int direction)
{
    if (direction > 0)
        return indexForward(index, length);
    else
        return indexBackward(index, length);
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
    while (vertices[minIndexR].y == minPointY)
    {
        minIndexR = indexForward(minIndexR, vertexList.length);
    }
    minIndexR = indexBackward(minIndexR, vertexList.length);

    while (vertices[minIndexL].y == minPointY)
    {
        minIndexL = indexBackward(minIndexL, vertexList.length);
    }
    minIndexL = indexForward(minIndexL, vertexList.length);
    
    cerr << "minPointY: " << minPointY << '\n';
    cerr << "maxPointY: " << maxPointY << '\n';
    cerr << "maxIndex: " << maxIndex << '\n';
    
    int leftEdgeDir = -1;
    const bool topIsFlat = (vertices[minIndexL].x != vertices[minIndexR].x);
    if (topIsFlat)
    {
        if (vertices[minIndexL].x > vertices[minIndexR].x)
        {
            leftEdgeDir = 1;
            std::swap(minIndexL, minIndexR);
        }
    }
    else
    {
        int nextIndex = indexForward(minIndexR, vertexList.length);
        int prevIndex = indexBackward(minIndexL, vertexList.length);
        int dxNext = vertices[nextIndex].x - vertices[minIndexL].x;
        int dyNext = vertices[nextIndex].y - vertices[minIndexL].y;
        int dxPrev = vertices[prevIndex].x - vertices[minIndexL].x;
        int dyPrev = vertices[prevIndex].y - vertices[minIndexL].y;
        if (dxNext * dyPrev < dyNext * dxPrev)
        {
            leftEdgeDir = 1;
            std::swap(minIndexL, minIndexR);
        }

        cerr << "nextIndex: " << nextIndex << '\n';
        cerr << "prevIndex: " << prevIndex << '\n';
        cerr << "leftEdgeDir: " << leftEdgeDir << '\n';
    }

    cerr << "minIndexL: " << minIndexL << '\n';
    cerr << "minIndexR: " << minIndexR << '\n';
    
    const int numScanlines = maxPointY - minPointY - 1 + int(topIsFlat);
    if (numScanlines <= 0)
        return false;
    const int yStart = minPointY + 1 - int(topIsFlat) + yOffset;

    vector<Scanline> scanlineList(numScanlines);
    
    int currIndex = minIndexL;
    int prevIndex = currIndex;
    bool skipFirst = !topIsFlat;
    do
    {
        indexMove(currIndex, vertexList.length, leftEdgeDir);
        scanEdge(vertices[prevIndex].x + xOffset, vertices[prevIndex].y,
                 vertices[currIndex].x + xOffset, vertices[currIndex].y,
                 1, skipFirst, scanlineList);
        prevIndex = currIndex;
        skipFirst = 0;
    } while (currIndex != maxIndex);
    
    currIndex = minIndexR;
    prevIndex = currIndex;
    skipFirst = !topIsFlat;
    do
    {
        indexMove(currIndex, vertexList.length, leftEdgeDir);
        scanEdge(vertices[prevIndex].x + xOffset - 1, vertices[prevIndex].y,
                 vertices[currIndex].x + xOffset - 1, vertices[currIndex].y,
                 0, skipFirst, scanlineList);
        prevIndex = currIndex;
        skipFirst = 0;
    } while (currIndex != maxIndex);
    
    drawScanlineList(scanlineList, color);
    
    return true;
}