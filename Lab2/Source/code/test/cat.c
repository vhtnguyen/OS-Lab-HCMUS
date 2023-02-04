#include "syscall.h"
#define maxlen 255

int main() {
    int id;
    int len;
    char filename[256];
    char s[256];

    PrintString("Nhap ten file ban muon doc: ");
    ReadString(filename, maxlen);
    
    id = Open(filename, 1);
    if (id != -1) {
        len = Seek(-1, id);
        Close(id);
        id = Open(filename, 0);
        Seek(0, id);
        Close(id);
        id = Open(filename, 0);
        Read(s, len, id);
        Close(id);
        PrintString("\nNoi dung file: \n");
        PrintString(s);
    }
    else
        PrintString("< ERROR > Khong the mo file de doc !!");
    Halt();
}