#include "syscall.h"
#define maxlen 32

int main()
{
    int len;
    char filename[maxlen + 1];
    /*Create a file*/
    
    if (Create("text.txt") == -1)
    {
        
    }
    else
    {
       
    }
    
    Halt();
}