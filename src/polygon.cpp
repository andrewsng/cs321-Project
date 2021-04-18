#include "vgaconst.h"
#include "math3d.h"
#include "polygon.h"

using std::vector;
#include <algorithm>
#include <optional>
using std::optional;
#include <tuple>
using std::tie;
#include <cmath>
using std::ceil;
#include <cstring>
using std::memset;
#include <dos.h>
#include <sys/nearptr.h>


Polygon2D::const_iterator Polygon2D::nextPointIter(const_iterator iter) const
{
    ++iter;
    if (iter == end())
        return begin();
    return iter;
}


Polygon2D::const_iterator Polygon2D::prevPointIter(Polygon2D::const_iterator iter) const
{
    if (iter == begin())
        return end() - 1;
    return iter - 1;
}


Polygon2D::const_iterator Polygon2D::shiftPointIter(Polygon2D::const_iterator iter,
    int direction) const
{
    if (direction > 0)
        return nextPointIter(iter);
    return prevPointIter(iter);
}


void scanEdge(int x0, int y0, int x1, int y1, bool setStart, bool skipFirst, 
    vector<Scanline> &scanlineList, int yStart)
{
    int dx = x1 - x0;
    int advanceAmount = dx > 0 ? 1 : -1;
    
    int height = y1 - y0;
    if (height <= 0)
        return;
    
    int width = std::abs(dx);
    if (width == 0)
    {
        for (int y = y0 + int(skipFirst); y < y1; ++y)
        {
            if (setStart)
                scanlineList[y-yStart].xStart = x0;
            else
                scanlineList[y-yStart].xEnd   = x0;
        }
    }
    else if (width == height)
    {
        if (skipFirst)
            x0 += advanceAmount;

        for (int y = y0 + int(skipFirst); y < y1; ++y)
        {
            if (setStart)
                scanlineList[y-yStart].xStart = x0;
            else
                scanlineList[y-yStart].xEnd   = x0;

            x0 += advanceAmount;
        }
    }
    else if (height > width)
    {
        int errorTerm = 0;
        if (dx < 0)
            errorTerm = -height + 1;
        if (skipFirst)
        {
            errorTerm += width;
            if (errorTerm > 0)
            {
                x0 += advanceAmount;
                errorTerm -= height;
            }
        }

        for (int y = y0 + int(skipFirst); y < y1; ++y)
        {
            if (setStart)
                scanlineList[y-yStart].xStart = x0;
            else
                scanlineList[y-yStart].xEnd   = x0;
            
            errorTerm += width;
            if (errorTerm > 0)
            {
                x0 += advanceAmount;
                errorTerm -= height;
            }
        }
    }
    else
    {
        int xMajorAdvanceAmount = (width / height) * advanceAmount;
        int errorTermAdvance = width % height;
        
        int errorTerm = 0;
        if (dx < 0)
            errorTerm = -height + 1;
        if (skipFirst)
        {
            x0 += xMajorAdvanceAmount;
            errorTerm += errorTermAdvance;
            if (errorTerm > 0)
            {
                x0 += advanceAmount;
                errorTerm -= height;
            }
        }

        for (int y = y0 + int(skipFirst); y < y1; ++y)
        {
            if (setStart)
                scanlineList[y-yStart].xStart = x0;
            else
                scanlineList[y-yStart].xEnd   = x0;
            
            x0 += xMajorAdvanceAmount;
            errorTerm += errorTermAdvance;
            if (errorTerm > 0)
            {
                x0 += advanceAmount;
                errorTerm -= height;
            }
        }
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
    uint8_t *screenPtr = (uint8_t *)SCREEN_ADR + __djgpp_conventional_base;
    screenPtr += yStart * SCREEN_WIDTH;
    for (int y = 0; y < scanlineList.size(); ++y)
    {
        int width = scanlineList[y].xEnd - scanlineList[y].xStart + 1;
        if (width > 0)
            memset(screenPtr + scanlineList[y].xStart, color, width);
        screenPtr += SCREEN_WIDTH;
    }
}


void drawScanlineListX(const vector<Scanline> &scanlineList, int color, int yStart)
{
    static const uint8_t leftClipPlaneMask[4]  = { 0x0F, 0x0E, 0x0C, 0x08 };
    static const uint8_t rightClipPlaneMask[4] = { 0x01, 0x03, 0x07, 0x0F };

    outportb(SC_INDEX, MAP_MASK);
    uint8_t *screenPtr = (uint8_t *)SCREEN_ADR + __djgpp_conventional_base;
    
    int yClipped = yStart;
    int size = scanlineList.size();
    if (yClipped < clipMinY)
    {
        size -= (clipMinY - yClipped);
        yClipped = clipMinY;
    }
    if (yClipped + size > clipMaxY)
    {
        size -= (yClipped + size - clipMaxY);
    }
    if (size <= 0)
        return;
    
    screenPtr += yClipped * X_WIDTH_BYTES + currentPageBase;
    for (int y = yClipped - yStart; y < yClipped - yStart + size; ++y)
    {
        int xStart = scanlineList[y].xStart;
        if (xStart < clipMinX)
            xStart = clipMinX;

        int xEnd = scanlineList[y].xEnd;
        if (xEnd >= clipMaxX)
            xEnd = clipMaxX - 1;
        
        if (xEnd < xStart)
        {
            screenPtr += X_WIDTH_BYTES;
            continue;
        }

        uint8_t *offset = screenPtr + (xStart >> 2);
        uint8_t leftMask = leftClipPlaneMask[xStart & 0x03];
        uint8_t rightMask = rightClipPlaneMask[xEnd & 0x03];
        int numAddresses = (xEnd - (xStart & ~0b011)) >> 2;
        if (numAddresses == 0)
            leftMask &= rightMask;
        
        outportb(SC_INDEX+1, leftMask);
        *offset++ = color;

        --numAddresses;
        if (numAddresses >= 0)
        {
            if (numAddresses > 0)
            {
                outportb(SC_INDEX+1, 0x0F);
                memset(offset, color, numAddresses);
            }
            outportb(SC_INDEX+1, rightMask);
            *(offset + numAddresses) = color;
        }
        
        screenPtr += X_WIDTH_BYTES;
    }
}


int leftEdgeDirection(const Polygon2D &poly, const Polygon2DAttribs &data)
{
    auto leftIter = data.minIndexL;
    auto rightIter = data.minIndexR;
    if (data.topIsFlat)
    {
        if (leftIter->x > rightIter->x)
        {
            return 1;
        }
    }
    else
    {
        auto nextIndex = poly.nextPointIter(rightIter);
        auto prevIndex = poly.prevPointIter(leftIter);
        int dxNext = nextIndex->x - leftIter->x;
        int dyNext = nextIndex->y - leftIter->y;
        int dxPrev = prevIndex->x - leftIter->x;
        int dyPrev = prevIndex->y - leftIter->y;
        if (dxNext * dyPrev < dyNext * dxPrev)
        {
            return 1;
        }
    }
    return -1;
}


optional<Polygon2DAttribs> getPolygonAttribs(const Polygon2D &poly, int yOffset)
{
    if (poly.empty())
        return std::nullopt;

    Polygon2DAttribs data;
    tie(data.minIndexL, data.maxIndex) = std::minmax_element(poly.begin(), poly.end(),
        [](const Point &a, const Point &b) {
            return a.y < b.y;
        });
    
    data.minY = data.minIndexL->y;
    data.maxY = data.maxIndex->y;
    if (data.minY == data.maxY)
        return std::nullopt;
    
    data.minIndexR = data.minIndexL;
    while (poly.nextPointIter(data.minIndexR)->y == data.minY)
    {
        data.minIndexR = poly.nextPointIter(data.minIndexR);
    }

    while (poly.prevPointIter(data.minIndexL)->y == data.minY)
    {
        data.minIndexL = poly.prevPointIter(data.minIndexL);
    }

    data.topIsFlat = data.minIndexL->x != data.minIndexR->x;
    data.leftEdgeDir = leftEdgeDirection(poly, data);
    if (data.leftEdgeDir > 0)
        std::swap(data.minIndexL, data.minIndexR);

    data.numScanlines = data.maxY - data.minY - 1 + int(data.topIsFlat);
    if (data.numScanlines <= 0)
        return std::nullopt;
    data.yStart = data.minY + 1 - int(data.topIsFlat) + yOffset;
    
    return optional<Polygon2DAttribs>(data);
}


void scanEdges(const Polygon2D &poly, const Polygon2DAttribs &attribs,
    vector<Scanline> &scanlines, int xOffset, int yOffset)
{
    auto currIndex = attribs.minIndexL;
    auto prevIndex = currIndex;
    bool skipFirst = !(attribs.topIsFlat);
    do
    {
        currIndex = poly.shiftPointIter(currIndex, attribs.leftEdgeDir);
        scanEdge(prevIndex->x + xOffset, prevIndex->y + yOffset,
                 currIndex->x + xOffset, currIndex->y + yOffset,
                 true, skipFirst, scanlines, attribs.yStart);
        prevIndex = currIndex;
        skipFirst = false;
    } while (currIndex != (attribs.maxIndex));
    
    currIndex = attribs.minIndexR;
    prevIndex = currIndex;
    skipFirst = !(attribs.topIsFlat);
    do
    {
        currIndex = poly.shiftPointIter(currIndex, -(attribs.leftEdgeDir));
        scanEdge(prevIndex->x + xOffset - 1, prevIndex->y + yOffset,
                 currIndex->x + xOffset - 1, currIndex->y + yOffset,
                 false, skipFirst, scanlines, attribs.yStart);
        prevIndex = currIndex;
        skipFirst = false;
    } while (currIndex != (attribs.maxIndex));
}


bool fillConvexPolygon(const Polygon2D &poly, int color, int xOffset, int yOffset)
{
    auto attribs = getPolygonAttribs(poly, yOffset);
    if (!attribs.has_value())
        return false;

    vector<Scanline> scanlineList(attribs->numScanlines);
    
    scanEdges(poly, *attribs, scanlineList, xOffset, yOffset);
    
    drawScanlineListX(scanlineList, color, attribs->yStart);
    
    return true;
}


void transformAndDrawPoly(const Mat4 &transform, const vector<Vec4> &poly, int color)
{
    Polygon2D projectedPoly(poly.size());

    for (int i = 0; i < projectedPoly.size(); ++i)
    {
        Vec4 transformed = transform * poly[i];

        const float projectionRatio = -2.0f;
        projectedPoly[i].x = int(transformed[0] / transformed[2]
            * projectionRatio * (SCREEN_WIDTH / 2.0f) + 0.5f) + (SCREEN_WIDTH / 2);
        projectedPoly[i].y = int(transformed[1] / transformed[2] * -1.0f
            * projectionRatio * (SCREEN_WIDTH / 2.0f) + 0.5f) + (SCREEN_HEIGHT / 2);
    }
    
    fillConvexPolygon(projectedPoly, color, 0, 0);
}