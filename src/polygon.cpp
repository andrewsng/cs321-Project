#include "vgaconst.h"
#include "polygon.h"

#include <vector>
using std::vector;
#include <cmath>
using std::ceil;
#include <dos.h>
#include <sys/nearptr.h>


void scanEdge(int x0, int y0, int x1, int y1, bool setStart, bool skipFirst, 
    vector<Scanline> &scanlineList, int yStart)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    if (dy <= 0)
        return;
    float inverseSlope = float(dx) / float(dy);
    
    for (int y = y0 + int(skipFirst); y < y1; ++y)
    {
        if (setStart)
            scanlineList[y-yStart].xStart = x0 + int(ceil((y-y0)*inverseSlope));
        else
            scanlineList[y-yStart].xEnd   = x0 + int(ceil((y-y0)*inverseSlope));
    }
}


void drawPixel(int x, int y, int color)
{
    static uint8_t *VGA = (uint8_t *)SCREEN_ADR + __djgpp_conventional_base;
    VGA[y * SCREEN_WIDTH + x] = uint8_t(color);
}


// drawPixelX
// Extremely slow pixel drawing (bit ops + 16-bit out) to demonstrate Mode X memory.
void drawPixelX(int x, int y, unsigned int pageBase, int color)
{
    static uint8_t *VGA = (uint8_t *)SCREEN_ADR + __djgpp_conventional_base;
    int offset = y * X_WIDTH_BYTES + (x >> 2) + pageBase;
    int plane = (0x0100 << (x & 0b11)) + MAP_MASK;
    outportw(SC_INDEX, plane);
    VGA[offset] = uint8_t(color);
}


void drawScanlineList(const vector<Scanline> &scanlineList, int color, int yStart)
{
    for (int y = 0; y < scanlineList.size(); ++y)
    {
        const int lineY = y + yStart;
        for (int x = scanlineList[y].xStart; x <= scanlineList[y].xEnd; ++x)
        {
            drawPixel(x, lineY, color);
        }
    }
}


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
    }

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
        currIndex = indexMove(currIndex, vertexList.length, leftEdgeDir);
        scanEdge(vertices[prevIndex].x + xOffset, vertices[prevIndex].y,
                 vertices[currIndex].x + xOffset, vertices[currIndex].y,
                 true, skipFirst, scanlineList, yStart);
        prevIndex = currIndex;
        skipFirst = false;
    } while (currIndex != maxIndex);
    
    currIndex = minIndexR;
    prevIndex = currIndex;
    skipFirst = !topIsFlat;
    do
    {
        currIndex = indexMove(currIndex, vertexList.length, -leftEdgeDir);
        scanEdge(vertices[prevIndex].x + xOffset - 1, vertices[prevIndex].y,
                 vertices[currIndex].x + xOffset - 1, vertices[currIndex].y,
                 false, skipFirst, scanlineList, yStart);
        prevIndex = currIndex;
        skipFirst = false;
    } while (currIndex != maxIndex);
    
    drawScanlineList(scanlineList, color, yStart);
    
    return true;
}