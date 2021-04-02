#ifndef POLYGON_H
#define POLYGON_H

#include <array>


struct Point
{
    int x{0};
    int y{0};
};


struct PointList
{
    int length{0};
    Point *points;
};


struct Scanline
{
    int xStart{0};
    int xEnd{0};
};


struct ScanlineList
{
    int length{0};
    int yStart{0};
    Scanline *lines;
};

void drawPixel(int x, int y, int color);

bool fillConvexPolygon(const PointList& vertexList, int color, int xOffset, int yOffset);


#endif  // POLYGON_H

