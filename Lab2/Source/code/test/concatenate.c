#include "syscall.h"
#define MAXLENGTH 256
char filename1[256];
char filename2[256];

char s1[256];
char s2[256];

// Kết quả nối của 2 file sẽ được thể hiện ở file resultCon.txt
int main() {
    int result;
    int id;
    int pos;
    PrintString("Nhap ten file nguon 1: ");
    ReadString(filename1,MAXLENGTH);

    PrintString("Nhap ten file nguon 2: ");
    ReadString(filename2,MAXLENGTH);
  
    id = Open(filename1, 1);
    if (id == -1)
    {
        PrintString("\n<ERROR> Mo file nguon 1 that bai !!");
        Halt();
        return 0;
    }

    Read(s1,MAXLENGTH,id);
    Close(id);

    id = Open(filename2, 1);
    if (id==-1)
    {
        PrintString("\n<ERROR> Mo file nguon 2 that bai !!");
        Halt();
        return 0;
    }

    Read(s2,MAXLENGTH,id);
    Close(id);
    
    // Tạo file lưu kết quả
    id = Open("resultCon.txt", 0);
    if (id != -1)
    {
        Close(id);
        Remove("resultCon.txt");
    }
    id = Create("resultCon.txt");
    Close(id);
    if (id == -1) {
        PrintString("\n<ERROR> Tao file that bai !!\n");
        Halt();
        return 0;
    }

    id = Open("resultCon.txt", 0);
    // Xác định kích thước
    pos = 0;
    while(s1[pos] != '\0') {
        pos++;
    }
    Write(s1, pos, id);  
    // Xác định kích thước
    pos = 0;
    while(s2[pos] != '\0') {
        pos++;
    }
    Write(s2, pos, id);

    if (Close(id) == 0)
    {
        PrintString("\nNoi file thanh cong\n");
    }
    else
    {
        PrintString("\n<ERROR> Noi file khong thanh cong !!");
    }

    Halt();
}