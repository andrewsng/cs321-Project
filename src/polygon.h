#ifndef POLYGON_H
#define POLYGON_H

#include <array>


struct Point
{
    int x;
    int y;
};


struct PointList
{
    int length;
    Point *points;
};


struct Scanline
{
    int xStart;
    int xEnd;
};


struct ScanlineList
{
    int length;
    int yStart;
    Scanline *lines;
};


bool fillConvexPolygon(const PointList& vertexList, int color, int xOffset, int yOffset);


#endif  // POLYGON_H

