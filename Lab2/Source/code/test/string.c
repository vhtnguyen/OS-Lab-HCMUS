#include "syscall.h"
char s[256];
#define MAXLENGTH 256
int main() {
    char ch;
    int x;
    int i = 0;

    // Đọc - In ra 1 chuỗi ký tự nhập từ bàn phím
    PrintString("\nNhap chuoi can in ra:\t");
    ReadString(s, MAXLENGTH);
    PrintString("Chuoi vua nhap la:\t");
    PrintString(s);

    Halt();
}