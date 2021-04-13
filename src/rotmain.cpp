#include "vgaconst.h"
#include "math3d.h"
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
#include <conio.h>
#include <sys/nearptr.h>


int main()
{
    if (!__djgpp_nearptr_enable())
        return -1;
    
    Mat4 worldTransform = {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, -140.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    };

    Mat4 viewTransform = {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    };
    
    const unsigned int pageStartOffsets[2] = {
        0, long(SCREEN_HEIGHT * SCREEN_WIDTH / 4)
    };
    
    int displayedPage = 0;
    int nonDisplayedPage = 0;

    setModeX();
    
    vector<Vec4> testPoly = {
        {-30, -15, 0, 1}, {0, 15, 0, 1}, {10 ,-5, 0, 1}
    };

    Point corners[4] = { {-10, -10}, {-10, 250}, {330, 250}, {330, -10} };
    const PointList screenRect{ 4, corners };
    
    Mat4 transform{};
    float rotation = M_PI / 60.0f;
    bool done = false;
    do {
        worldTransform[0][0] = cosf(rotation);
        worldTransform[2][2] = worldTransform[0][0];
        worldTransform[0][2] = sinf(rotation);
        worldTransform[2][0] = -worldTransform[0][2];

        transform = viewTransform * worldTransform;
        

        fillConvexPolygon(screenRect, 0, 0, 0);
        
        transformAndDrawPoly(transform, testPoly, testPoly.size(), 9);
        
        rotation += (M_PI / 30.0f);
        if (rotation >= (2 * M_PI))
            rotation -= (2 * M_PI);
        if (kbhit())
        {
            switch (getch())
            {
                case 0x1B:
                    done = true; break;
                case 'A': case 'a':
                    worldTransform[2][3] -= 3.0f; break;
                case 'T': case 't':
                    if (worldTransform[2][3] < -40.0f)
                        worldTransform[2][3] += 3.0f; break;
                case 0:
                    switch(getch())
                    {
                        case 0x4B:
                            worldTransform[0][3] -= 3.0; break;
                        case 0x4D:
                            worldTransform[0][3] += 3.0; break;
                        case 0x48:
                            worldTransform[1][3] += 3.0; break;
                        case 0x50:
                            worldTransform[1][3] -= 3.0; break;
                        default:
                            break;
                    }
                    break;
                default:
                    getch(); break;
            }
        }
    } while (!done);

    setModeText();
    
    __djgpp_nearptr_disable();
    
    return 0;
}