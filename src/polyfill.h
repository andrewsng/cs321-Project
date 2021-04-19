// polyfill.h
// Andrew S. Ng
// Created: 2021-04-18
// 
// For CS 321 DOS VGA Graphics
// Header for polygon filling routines

#ifndef POLYFILL_H
#define POLYFILL_H

#include "polygon.h"
#include "object.h"
#include "math3d.h"

#include <vector>


extern int clipMinX;
extern int clipMaxX;
extern int clipMinY;
extern int clipMaxY;
extern unsigned int currentPageBase;


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

bool fillConvexPolygon(const Polygon2D &poly, int color, int xOffset, int yOffset);

void transformAndDrawPoly(const Mat4 &transform, const std::vector<Vec4> &poly, int color);

std::vector<Point> transformAndProjectObject(const Mat4 &transform, const Object &obj);


#endif  // POLYFILL_H