// palettemain.cpp
// Andrew S. Ng
// Created: 2021-04-23
// 
// For CS 321 DOS VGA Graphics
// Program to display color palette

#include "vgaconst.h"
#include "polyfill.h"
#include "modes.h"

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


    setMode13h();
    
    for (int y = 0; y < SCREEN_HEIGHT; ++y)
    {
        for (int x = 0; x < SCREEN_WIDTH; ++x)
        {
            drawPixel(x, y, std::min(x, 255));
        }
    }

    getch();

    setModeText();
    
    __djgpp_nearptr_disable();
    
    return 0;
}