#include "vgaconst.h"
#include "polygon.h"
#include "modes.h"

#include <iostream>
using std::cerr;
#include <cstdint>
using std::uint8_t;
#include <dos.h>
#include <sys/nearptr.h>


int main()
{
    if (!__djgpp_nearptr_enable())
        return -1;

    const int green  = 10;
    const int red    = 12;
    const int purple = 13;

    
    const int numVertices = 3;
    Point v1[numVertices] = { {100, 50}, {50, 150}, {150, 100} };
    const PointList t1{ numVertices, v1 };
    Point v2[numVertices] = { {100, 50}, {125, 25}, {150, 100} };
    const PointList t2{ numVertices, v2 };
    drawPixel(100,  50, red);
    drawPixel( 50, 150, red);
    drawPixel(150, 100, red);


    setMode13h();
    
    fillConvexPolygon(t1, green, 0, 0);
    fillConvexPolygon(t2, purple, 0, 0);
    getchar();

    setModeX();
    fillConvexPolygon(t1, green, 0, 0);
    fillConvexPolygon(t2, purple, 0, 0);
    getchar();

    setModeText();
    
    __djgpp_nearptr_disable();
    
    return 0;
}