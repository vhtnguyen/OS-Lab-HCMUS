#include "syscall.h"

int main()
{
    int i;
    char ch;
    PrintString("\nBANG MA ASCII CAC KY TU DOC DUOC\n");
    for (i = 32; i < 127; i++)
    {
        PrintNum(i);
        PrintChar('\t');
        ch = i;
        PrintChar(ch);
        PrintChar('\n');
    }
    Halt();
}