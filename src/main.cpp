#include <dos.h>

int main()
{
    union REGS regs;
    
    regs.h.ah = 0x00;
    regs.h.al = 0x13;
    int86(0x10, &regs, &regs);
    
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