#include "syscall.h"
char s[256];
#define MAXLENGTH 256
int main() {
    char ch;
    int x;
    int i = 0;

    // tạo số nguyên ngẫu nhiên
    for (; i < 10; ++i) {
        PrintString("\nSo nguyen ngau nhien:\t");
        PrintNum(RandomNum());
    }

    Halt();
}