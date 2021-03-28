#include <cstdint>
using std::uint8_t;
#include <dos.h>
#include <sys/nearptr.h>


int main()
{
    if (!__djgpp_nearptr_enable())
        return -1;

    union REGS regs;
    
    regs.h.ah = 0x00;
    regs.h.al = 0x13;
    int86(0x10, &regs, &regs);
    
    const int width = 320;
    const int height = 200;
    
    const int color = 10;
    regs.h.ah = 0x0C;
    regs.h.al = color;
    regs.x.cx = width / 2;
    regs.x.dx = height / 2;
    int86(0x10, &regs, &regs);
    
    uint8_t *VGA = (uint8_t *)0xA0000 + __djgpp_conventional_base;
    const int x = 200;
    const int y = height / 2;
    const int offset = y * width + x;
    VGA[offset] = 12;
    
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const int offset = y * width + x;
            VGA[offset] = x % 256;
        }
    }
    
    // wait ~1 second
    int sum = 0;
    for (int i = 0; i < 100000000; ++i)
    {
        sum += i;
    }

    regs.h.ah = 0x00;
    regs.h.al = 0x03;
    int86(0x10, &regs, &regs);
    
    __djgpp_nearptr_disable();
    
    return 0;
}