#include "syscall.h"
int arr[100];

int main() {
    int n;
    int choose;
    int i;
    int j;
    int temp;
    int swapped;
    swapped = 0;

    // Nhập số lượng phần tử
    PrintString("Nhap so phan tu cua mang: \t");
    n = ReadNum();
    while (n <= 0 || n > 100) {
        PrintString("Nhap so phan tu cua mang: \t");
        n = ReadNum();
    }

    // Nhập giá trị các phần tử
    for (i = 0; i < n; i++) {
        PrintString("a[");
        PrintNum(i);
        PrintString("] = \t");
        arr[i] = ReadNum();
    }

    // Nhập lựa chọn sắp xếp
    PrintString("Chon cach sap xep (0: Tang dan, 1: Giam dan): \t");
    choose = ReadNum();

    // Sắp xếp tăng dần theo BubbleSort
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }
        if (swapped == 0) {
            break;
        }
    }

    // Xuất mảng đã sắp xếp
    switch (choose) {
    case 0: {
        PrintString("Mang sau sap xep tang dan: \t");
        for (i = 0; i < n; i++) {
            PrintNum(arr[i]);
            PrintString("\t");
        }
        break;
    }
    case 1: {
        PrintString("Mang sau sap xep giam dan: \t");
        for (i = n - 1; i >= 0; i--) {
            PrintNum(arr[i]);
            PrintString("\t");
        }
        break;
    }

    default:
        PrintString("Lua chon khong hop le. \t");
        break;
    }

    Halt();
}