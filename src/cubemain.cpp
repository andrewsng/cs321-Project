// cubemain.cpp
// Andrew S. Ng
// Created: 2021-04-18
// 
// For CS 321 DOS VGA Graphics
// Rotating cube program
// to test 3D polygon drawing

#include "vgaconst.h"
#include "math3d.h"
#include "polygon.h"
#include "polyfill.h"
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


unsigned int currentPageBase = 0;
int clipMinX = 0;
int clipMaxX = 0;
int clipMinY = 0;
int clipMaxY = 0;

int main()
{
    if (!__djgpp_nearptr_enable())
        return -1;
    
    const unsigned int pageStartOffsets[2] = {
        0, SCREEN_HEIGHT * SCREEN_WIDTH / 4
    };
    
    int displayedPage = 0;
    int nonDisplayedPage = 0;
    
    Mat4 worldTransform = {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, -100.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    };

    Mat4 viewTransform = {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    };

    vector<Vec4> cubeVertices{
        {  15, 15, 15, 1 }, {  15, 15, -15, 1 }, {  15, -15, 15, 1 }, {  15, -15, -15, 1 }, 
        { -15, 15, 15, 1 }, { -15, 15, -15, 1 }, { -15, -15, 15, 1 }, { -15, -15, -15, 1 }
    };
    
    vector<std::size_t> face1{ 1, 3, 2, 0 };
    vector<std::size_t> face2{ 5, 7, 3, 1 };
    vector<std::size_t> face3{ 4, 5, 1, 0 };
    vector<std::size_t> face4{ 3, 7, 6, 2 };
    vector<std::size_t> face5{ 5, 4, 6, 7 };
    vector<std::size_t> face6{ 0, 2, 6, 4 };
    vector<Face> cubeFaces{
        { face1, 15 }, { face2, 14 }, { face3, 12 },
        { face4, 11 }, { face5, 10 }, { face6,  9 }
    };
    
    Object cube{ cubeVertices, cubeFaces };

    const Polygon2D screenRect{ {-10, -10}, {-10, 250}, {330, 250}, {330, -10} };
    
    Mat4 transform{};
    std::vector<Point> screenPoints{};
    const float rotation = M_PI / 30.0f;
    bool done = false;
    bool needRecalcTransform = true;

    setModeX();
    showPage(pageStartOffsets[displayedPage]);
    
    do {

        if (needRecalcTransform)
        {
            transform = viewTransform * worldTransform;
            screenPoints = transformAndProjectObject(transform, cube);
            needRecalcTransform = false;
        }
        
        nonDisplayedPage = displayedPage ^ 1;
        currentPageBase = pageStartOffsets[nonDisplayedPage];

        fillConvexPolygon(screenRect, 0, 0, 0);
        
        drawVisibleFaces(cube, screenPoints);
        
        displayedPage = nonDisplayedPage;
        showPage(displayedPage);
        
        while (kbhit())
        {
            switch (getch())
            {
                case 0x1B:
                    done = true; break;
                case 'A': case 'a':
                    worldTransform[2][3] -= 3.0f;
                    needRecalcTransform = true;
                    break;
                case 'T': case 't':
                    if (worldTransform[2][3] < -40.0f)
                    {
                        worldTransform[2][3] += 3.0f;
                        needRecalcTransform = true;
                    }
                    break;
                case '4':
                    worldTransform = appendRotationY(worldTransform, -rotation);
                    needRecalcTransform = true;
                    break;
                case '6':
                    worldTransform = appendRotationY(worldTransform, rotation);
                    needRecalcTransform = true;
                    break;
                case '8':
                    worldTransform = appendRotationX(worldTransform, -rotation);
                    needRecalcTransform = true;
                    break;
                case '2':
                    worldTransform = appendRotationX(worldTransform, rotation);
                    needRecalcTransform = true;
                    break;
                case 0:
                    switch(getch())
                    {
                        case 0x3B:
                            worldTransform = appendRotationZ(worldTransform, rotation);
                            needRecalcTransform = true;
                            break;
                        case 0x3C:
                            worldTransform = appendRotationZ(worldTransform, -rotation);
                            needRecalcTransform = true;
                            break;
                        case 0x4B:
                            worldTransform[0][3] -= 3.0;
                            needRecalcTransform = true;
                            break;
                        case 0x4D:
                            worldTransform[0][3] += 3.0;
                            needRecalcTransform = true;
                            break;
                        case 0x48:
                            worldTransform[1][3] += 3.0;
                            needRecalcTransform = true;
                            break;
                        case 0x50:
                            worldTransform[1][3] -= 3.0;
                            needRecalcTransform = true;
                            break;
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