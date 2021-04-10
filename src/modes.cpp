#include "modes.h"

#include <cstdint>
using std::uint8_t;
using std::uint16_t;
#include <cstring>
#include <dos.h>
#include <sys/nearptr.h>


// setModeX (320x240, 256-color)
// Pre:
//   __djgpp_nearptr_enable() has been successfully called.
void setModeX()
{
    const uint16_t SC_INDEX   = 0x03C4;
    const uint16_t CRTC_INDEX = 0x03D4;
    const uint16_t CRTC_DATA  = 0x03D5;
    const uint16_t MISC_INDEX = 0x03C2;
    const uint16_t SCREEN_SEG = 0xA000;
    
    const int numCRTParms = 10;
    const uint16_t CRTParms[numCRTParms] = {
        0x0D06, 0x3E07, 0x4109, 0xEA10, 0xAC11,
        0xDF12, 0x0014, 0xE715, 0x0616, 0xE317
    };

    union REGS regs;
    regs.h.ah = 0x00;
    regs.h.al = 0x13;
    int86(0x10, &regs, &regs);
    
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

    uint8_t *VGA = (uint8_t *)0xA0000 + __djgpp_conventional_base;
    std::memset(VGA, 0, 320*240);
}