#include "syscall.h"
#define MAXLENGTH 255
char filename[256];
int main() {
    int result;
    PrintString("Nhap ten file can delete: ");
    ReadString(filename, MAXLENGTH);

    result = Remove(filename);

    if (result == 0) {
        PrintString("\nXoa file khong thanh cong.");
    }
    if (result == 1) {
        PrintString("\nXoa file thanh cong.");
    }
    Halt();
}