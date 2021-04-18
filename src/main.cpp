#include "vgaconst.h"
#include "polygon.h"
#include "modes.h"

#include <iostream>
using std::cerr;
#include <vector>
using std::vector;
#include <cstdint>
using std::uint8_t;
#include <cmath>
#include <dos.h>
#include <sys/nearptr.h>


unsigned int currentPageBase = 0;
int clipMinX = 0;
int clipMaxX = 0;
int clipMinY = 0;
int clipMaxY = 0;

int main()
{
    if (!__djgpp_nearptr_enable())
        return -1;

    const int green  = 10;
    const int red    = 12;
    const int purple = 13;

    const unsigned int pageStartOffsets[2] = {
        0, SCREEN_HEIGHT * SCREEN_WIDTH / 4
    };
    
    int displayedPage = 0;
    int nonDisplayedPage = 0;

    // setMode13h();
    
    // fillConvexPolygon(t1, green, 0, 0);
    // fillConvexPolygon(t2, purple, 0, 0);
    // getchar();

    setModeX();
    showPage(pageStartOffsets[displayedPage]);
    
    for (int i = 0; i < 1000; ++i)
    {
        nonDisplayedPage = displayedPage ^ 1;
        currentPageBase = pageStartOffsets[nonDisplayedPage];
        
        const vector<Point> r0{ {0, 0}, {0, 240}, {320, 240}, {320, 0} };
        fillConvexPolygon(r0, 0, 0, 0);

        const int numVertices = 3;
        const vector<Point> t1{ {100, 50}, {50, 150}, {150, 100} };
        const vector<Point> t2{ {100, 50}, {125, 25}, {150, 100} };
        int dy = 100 * std::sin(i * 3.14159265358979 / 180.);

        fillConvexPolygon(t1, green, dy, dy);
        fillConvexPolygon(t2, purple, dy, dy);
        
        displayedPage = nonDisplayedPage;
        showPage(displayedPage);
    }

    setModeText();
    
    __djgpp_nearptr_disable();
    
    return 0;
}