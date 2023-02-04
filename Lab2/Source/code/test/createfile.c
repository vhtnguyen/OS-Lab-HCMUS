#include "syscall.h"
#define maxlen 32

int main() {
    int len, id;
    char *filename;

    do {
        PrintString("Nhap ten file muon tao: ");
        ReadString(filename, maxlen);
        id = Open(filename, 0);
        if (id != -1)
            PrintString("< ERROR > File da ton tai !! Vui long nhap ten khac\n");
        Close(id);
    } while (id != -1);

    if (Create(filename) == -1)
        PrintString("< ERROR > Tao file khong thanh cong !!");
    else
        PrintString("Tao file thanh cong !!");

    Halt();
}