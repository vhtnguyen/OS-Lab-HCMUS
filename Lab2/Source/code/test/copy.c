#include "syscall.h"
#define maxlen 255
char s[256];
int main() {
    int src, dst;
    int len;
    char filenameSource[256];
    char filenameDestinate[256];

    PrintString("Nhap ten file nguon: ");
    ReadString(filenameSource, maxlen);

    PrintString("Nhap ten file dich: ");
    ReadString(filenameDestinate, maxlen);

    src = Open(filenameSource, 1);
    if (src != -1) {
        len = Seek(-1, src);
        Seek(0, src);
        len = Read(s, len, src);
        Close(src);
        PrintString("\nSo ki tu da doc duoc: ");
        PrintNum(len);

        dst = Open(filenameDestinate, 0);
        if (dst == -1) {
            if (Create(filenameDestinate) == -1) {
                PrintString("\n< ERROR > Khong the tao file dich !! Chuong trinh huy bo !!");
                Halt();
            }
            else {
                PrintString("\nTao file dich thanh cong !!");
                dst = Open(filenameDestinate, 0);
            }
        }
        Write(s, len, dst);
        PrintString("\nCopy du lieu thanh cong !!");
        Close(dst);
    }
    else
        PrintString("\n< ERROR > File nguon khong ton tai !!");

    Halt();
}