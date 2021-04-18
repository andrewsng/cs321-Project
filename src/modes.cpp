// modes.cpp
// Andrew S. Ng
// Created: 2021-04-09
// 
// For CS 321 DOS VGA Graphics
// Source for mode setting routines

#include "vgaconst.h"
#include "modes.h"
#include "polyfill.h"

#include <cstdint>
using std::uint8_t;
using std::uint16_t;
#include <cstring>
#include <dos.h>
#include <sys/nearptr.h>


void setVideoMode(uint8_t mode)
{
    union REGS regs;
    regs.h.ah = 0x00;
    regs.h.al = mode;
    int86(0x10, &regs, &regs);
}


void setModeText()
{
    setVideoMode(0x03);
}


unsigned int SCREEN_WIDTH  = 0;
unsigned int SCREEN_HEIGHT = 0;


void setClipBounds()
{
    clipMinX = 0;
    clipMaxX = SCREEN_WIDTH;
    clipMinY = 0;
    clipMaxY = SCREEN_HEIGHT;
}


// setMode13h (320x200, 256-color)
void setMode13h()
{
    SCREEN_WIDTH  = 320;
    SCREEN_HEIGHT = 200;
    setClipBounds();

    setVideoMode(0x13);
}


// setModeX (320x240, 256-color)
// Pre:
//   __djgpp_nearptr_enable() has been successfully called.
void setModeX()
{
    static const int numCRTParms = 10;
    static const uint16_t CRTParms[numCRTParms] = {
        0x0D06, 0x3E07, 0x4109, 0xEA10, 0xAC11,
        0xDF12, 0x0014, 0xE715, 0x0616, 0xE317
    };

    setMode13h();

    SCREEN_WIDTH  = 320;
    SCREEN_HEIGHT = 240;
    setClipBounds();

    
    outportw(SC_INDEX, 0x0604);
    outportw(SC_INDEX, 0x0100);

    outportb(MISC_INDEX, 0xE3);
    outportw(SC_INDEX, 0x0300);
    
    outportb(CRTC_INDEX, 0x11);
    auto data = inportb(CRTC_DATA) & 0x7F;
    outportb(CRTC_DATA, data);
    
    for (int i = 0; i < numCRTParms; ++i)
        outportw(CRTC_INDEX, CRTParms[i]);
    
    outportw(SC_INDEX, 0x0F02);

    uint8_t *VGA = (uint8_t *)SCREEN_ADR + __djgpp_conventional_base;
    std::memset(VGA, 0, SCREEN_WIDTH*SCREEN_HEIGHT);
}


void showPage(uint16_t startOffset)
{
    uint16_t low = START_ADDR_LOW | (startOffset & 0xFF00);
    uint16_t high = START_ADDR_HIGH | (startOffset & 0x00FF << 8);
    
    while ((inportb(INPUT_STATUS_1) & 0x01) != 0);

    outportw(CRTC_INDEX, low);
    outportw(CRTC_INDEX, high);
    
    while ((inportb(INPUT_STATUS_1) & 0x08) == 0);
}

