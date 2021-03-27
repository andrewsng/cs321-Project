#include <dos.h>

int main()
{
    union REGS regs;
    
    regs.h.ah = 0x00;
    regs.h.al = 0x13;
    int86(0x10, &regs, &regs);
    
    const int color = 10;
    regs.h.ah = 0x0C;
    regs.h.al = color;
    regs.x.cx = 160;
    regs.x.dx = 100;
    int86(0x10, &regs, &regs);
    
    // wait ~1 second
    int sum = 0;
    for (int i = 0; i < 100000000; ++i)
    {
        sum += i;
    }

    regs.h.ah = 0x00;
    regs.h.al = 0x03;
    int86(0x10, &regs, &regs);
    
    return 0;
}