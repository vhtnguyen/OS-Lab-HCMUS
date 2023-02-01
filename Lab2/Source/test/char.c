#include "syscall.h"
char s[256];
#define MAXLENGTH 256
int main() {
    char ch;
    int x;
    int i = 0;

    // Đọc - In ra 1 ký tự nhập từ bàn phím
    PrintString("\nNhap ki tu can in ra:\t");
    ch = ReadChar();
    PrintString("Ki tu vua nhap la:\t");
    PrintChar(ch);

    Halt();
}