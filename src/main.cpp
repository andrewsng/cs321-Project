#include "vgaconst.h"
#include "polygon.h"
#include "modes.h"

#include <iostream>
using std::cerr;
#include <cstdint>
using std::uint8_t;
#include <cmath>
#include <dos.h>
#include <sys/nearptr.h>


int main()
{
    if (!__djgpp_nearptr_enable())
        return -1;

    const int green  = 10;
    const int red    = 12;
    const int purple = 13;

    // setMode13h();
    
    // fillConvexPolygon(t1, green, 0, 0);
    // fillConvexPolygon(t2, purple, 0, 0);
    // getchar();

    setModeX();
    for (int i = 0; i < 10000; ++i)
    {
        int dy = 100 * std::sin(i * 3.14159265358979 / 180.);
        const int numVertices = 3;
        Point v1[numVertices] = { {100, 50}, {50, 150}, {150, 100} };
        const PointList t1{ numVertices, v1 };
        Point v2[numVertices] = { {100, 50}, {125, 25}, {150, 100} };
        const PointList t2{ numVertices, v2 };
        Point v3[4] = { {0, 0}, {0, 240}, {320, 240}, {320, 0} };
        const PointList r0{ 4, v3 };
        fillConvexPolygon(t1, green, dy, dy);
        fillConvexPolygon(t2, purple, dy, dy);
        fillConvexPolygon(r0, 0, 0, 0);
    }

    setModeText();
    
    __djgpp_nearptr_disable();
    
    return 0;
}