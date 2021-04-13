#ifndef POLYGON_H
#define POLYGON_H

#include "math3d.h"

#include <vector>


extern int clipMinX;
extern int clipMaxX;
extern int clipMinY;
extern int clipMaxY;
extern unsigned int currentPageBase;


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

void drawPixelX(int x, int y, unsigned int pageBase, int color);

bool fillConvexPolygon(const PointList& vertexList, int color, int xOffset, int yOffset);

void transformAndDrawPoly(const Mat4 &transform, const std::vector<Vec4> &poly, int polyLength, int color);


#endif  // POLYGON_H

