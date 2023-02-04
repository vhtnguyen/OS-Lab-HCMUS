#include "syscall.h"
char s[256];
#define MAXLENGTH 256
int main() {
    int num;
    int x;
    int i = 0;

    // Đọc - In ra 1 ký tự nhập từ bàn phím
    PrintString("\nNhap so can in ra:\t");
    num = ReadNum();
    PrintString("So vua nhap la:\t");
    PrintNum(num);

    Halt();
}