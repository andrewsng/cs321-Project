#ifndef POLYGON_H
#define POLYGON_H

#include "math3d.h"

#include <vector>
#include <initializer_list>


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


class Polygon2D
{

public:
    
    using iterator = std::vector<Point>::iterator;
    using const_iterator = std::vector<Point>::const_iterator;
    
    Polygon2D(std::size_t size)
        :_points(size)
    {}
    
    Polygon2D(std::initializer_list<Point> points)
        :_points(points)
    {}
    
    bool empty() const { return _points.empty(); }
    
    std::size_t size() const { return _points.size(); }
    
    iterator begin() { return _points.begin(); }
    const_iterator begin() const { return _points.begin(); }
    
    iterator end() { return _points.end(); }
    const_iterator end() const { return _points.end(); }
    
    Point &operator[](std::size_t index) { return _points[index]; }
    const Point &operator[](std::size_t index) const { return _points[index]; }
    
    const_iterator nextPointIter(const_iterator iter) const;

    const_iterator prevPointIter(const_iterator iter) const;

    const_iterator shiftPointIter(const_iterator iter, int direction) const;

private:
    
    std::vector<Point> _points;

};


struct Polygon2DAttribs

{
    Polygon2D::const_iterator minIndexL;
    Polygon2D::const_iterator minIndexR;
    Polygon2D::const_iterator maxIndex;
    int minY;
    int maxY;
    int leftEdgeDir;
    bool topIsFlat;
    int numScanlines;
    int yStart;
};


void drawPixel(int x, int y, int color);

void drawPixelX(int x, int y, unsigned int pageBase, int color);

bool fillConvexPolygon(const Polygon2D &poly, int color, int xOffset, int yOffset);

void transformAndDrawPoly(const Mat4 &transform, const std::vector<Vec4> &poly, int color);


#endif  // POLYGON_H

