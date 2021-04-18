// polygon.cpp
// Andrew S. Ng
// Created: 2021-04-01
// 
// For CS 321 DOS VGA Graphics
// Source for Polygon types

#include "polygon.h"


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

