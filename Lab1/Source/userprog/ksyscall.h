/////////////////////////////////////////////////
// 	DH KHTN - DHQG TPHCM						/
// 	20120307		Phạm Gia Khiêm				/
// 	20120519		Nguyễn Thị Thúy Liễu		/
// 	20120540		Võ Hoàng Thảo Nguyên		/
/////////////////////////////////////////////////

/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"
#include "synchconsole.h"
#include "syscall.h"

#define MAXLENGTH 256
#define MODULUS 2147483647
#define CONST_C 16807
#define INT_MIN (-2147483647 - 1)

bool FIRSTRAND = true;
int RAND_NUM = 0;

// Input: - User space address (int)
// - Limit of buffer (int)
// Output:- Buffer (char*)
// Purpose: Copy buffer from User memory space to System memory space
char* User2System(int virtAddr, int limit)
{
    int i; // index
    int oneChar;
    char *kernelBuf = NULL;
    kernelBuf = new char[limit + 1]; // need for terminal string

    if (kernelBuf == NULL)
        return kernelBuf;
    memset(kernelBuf, 0, limit + 1);

    for (i = 0; i < limit; i++)
    {
        kernel->machine->ReadMem(virtAddr + i, 1, &oneChar);
        kernelBuf[i] = (char)oneChar;
        if (oneChar == 0)
            break;
    }
    return kernelBuf;
}

// Input: - User space address (int)
// - Limit of buffer (int)
// - Buffer (char[])
// Output:- Number of bytes copied (int)
// Purpose: Copy buffer from System memory space to User memory space
int System2User(int virtAddr, char *buffer, int len)
{
    if (len <= 0)
        return len;
    int i = 0;
    int oneChar = 0;
    do
    {
        oneChar = (int)buffer[i];
        kernel->machine->WriteMem(virtAddr + i, 1, oneChar);
        i++;
    } while (i < len && oneChar != 0);
    return i;
}

void IncreasePC()
/* Modify return point */
{
    /* set previous programm counter (debugging only)*/
    kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

    /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
    kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

    /* set next programm counter for brach execution */
    kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}
// -------------------------------------------------------------------

void SysHalt() {
    kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2) {
    return op1 + op2;
}

int SysSub(int op1, int op2) {
    return op1 - op2;
}

// Các hàm xử lý trong Project ------------------------------------------------

void SysReadChar()
{
    // Lấy kí tự từ người dùng nhập vào
    char ch = kernel->synchConsoleIn->GetChar();
    // Nhận lệnh xuống dòng (enter)
    while (ch != '\n' && kernel->synchConsoleIn->GetChar() != '\n') {
        // Do nothing !!
    }    
    // Ghi dữ liệu ký tự vào reg2 
    kernel->machine->WriteRegister(2, ch);
}

void SysPrintChar()
{
    // Lấy dữ liệu từ reg4
    char ch = (char)kernel->machine->ReadRegister(4);
    // Đưa lên system
    kernel->synchConsoleOut->PutChar(ch);
}

void SysReadString(int length)
{
     // Tạo chuỗi ký tự buffer
    char *buffer = new char[length + 1];
    // Nếu buffer == NULL thì kết thúc (Do không đủ vùng nhớ để tạo)
    if (!buffer)
        return;

    // Vị trí i (index) của từng ký tự
    int i = 0;
    // Biến ch có kiểu char
    char ch;

    // Dùng vòng lặp để đọc chuỗi ký tự nhập vào
    while (i < length) {
        do {
            // Nhận ký tự từ system
            ch = kernel->synchConsoleIn->GetChar();
        } while (ch == EOF);
        // Khi nhập: Enter -> ket thuc nhap
        if (ch == '\0' || ch == '\n') {
            buffer[i] = '\0';
            break;
        }

        // Lưu ký tự vào chuỗi ký tự buffer
        buffer[i++] = ch;
    }

    // kiểm soát trường hợp tràn buffer;
    if (ch != '\0' && ch != '\n') {
        while (true) {
            ch = kernel->synchConsoleIn->GetChar();
            if (ch == '\0' || ch == '\n')
                break;
        }
    }

    // Gọi ptr là value của reg4
    int ptr = kernel->machine->ReadRegister(4);
    // Chuyển dữ liệu vùng nhớ buffer từ hệ thống (system) sang giao diện người dùng (user)
    // Với virAdd = ptr (giá trị reg4)
    //     buffer = buffer (dữ liệu của buffer từ system sẽ chuyển thành buffer của user)
    //     len = MAXLENGTH (Độ dài của buffer là MAXLENGTH)
    System2User(ptr, buffer, length);
    delete[] buffer;
}

void SysPrintString()
{
    // Vị trí (index) của ký tự
    int i = 0;
    // Lấy giá trị (value) từ reg4
    int ptr = kernel->machine->ReadRegister(4);

    // Đọc dữ liệu vùng nhớ từ giao diện người dùng (user)
    // Với virAdd = ptr (giá trị reg4)
    //     len = MAXLENGTH (độ dài của buffer người dùng)
    // Trả về giá trị char* (chuỗi dữ liệu) từ người dùng (user) gán vào char* buffer của hệ thống (system)
    char* buffer = User2System(ptr, MAXLENGTH);
    
    // Dùng vòng lặp để đưa lên hệ thống (system)
    while (buffer[i] != '\0') {
        kernel->synchConsoleOut->PutChar(buffer[i++]);
    }
    delete[] buffer;
}

// https://en.wikipedia.org/wiki/Linear_congruential_generator
void SysRandomNum()
{
    int seed = kernel->stats->totalTicks;
    if (FIRSTRAND) {
        RAND_NUM = seed;
        FIRSTRAND = false;
    }
    RAND_NUM = (RAND_NUM * seed + CONST_C) % MODULUS;
    if (RAND_NUM < 0) {
        RAND_NUM = -(RAND_NUM);
    }
    kernel->machine->WriteRegister(2, RAND_NUM);
}

void SysReadNum()
{
    /*
        Input   : NULL
        Output  : Số nguyên Int
        CN      : Đọc số nguyên từ bàn phím
    */
    // Tạo chuỗi buffer chứa number
    char *buffer = new char[MAXLENGTH + 1];
    // Nếu buffer == NULL -> kết thúc
    if (!buffer)
        return;

    // Vị trí i (index) của từng ký tự trong buffer
    int i = 0;
    // Biến ch có kiểu char
    char ch;
    // Kiểm tra int 
    bool isInt = true;
    // Kiểm tra số - 
    bool isNegative = false;
    // Lưu kết quả trả về
    int result = 0;

    // Dùng vòng lặp để đọc chuỗi ký tự nhập vào 
    while (i < MAXLENGTH)
    {
        do
        {
            // Nhận ký tự từ system
            ch = kernel->synchConsoleIn->GetChar();
        } while (ch == EOF);
        // Khi nhập: Enter -> ket thuc nhap
        if (ch == '\0' || ch == '\n') 
            break;
        
        // Lưu ký tự vào chuỗi ký tự buffer
        buffer[i++] = ch;
    }
    /*
            Trong quá trình test, có xuất hiện 1 bug mà chúng em chưa hiểu:
        Khi nhập vào đến ký tự thứ 8 thì strlen() bị lỗi gì đó và tự động
        lấy độ dài của chuỗi là 17, với mọi trường hợp, còn khi số ký tự dưới
        7 thì strlen() hoạt động đúng với cơ chế của nó.
            Tụi em thử fix và thêm phần tử \0 xuống buffer ở vị trí thứ i
            Và nó đã hoạt động tốt.
    */
    buffer[i] = '\0';

    // Xoá bớt kí tự trắng ở đầu (nếu có)
    while (buffer[0] == ' ') {
        int n = strlen(buffer);
        for (int j = 0; j < n - 1; j++) {
            buffer[j] = buffer[j + 1];
        }
        buffer[n - 1] = '\0';
    }

    // xoá bớt kí tự trắng ở cuối (nếu có)
    while (buffer[strlen(buffer) - 1] == ' ' && strlen(buffer) > 0) {
        buffer[strlen(buffer) - 1] = '\0';
    }

    // Nếu người dùng không nhập thì xuất ra là 0
    if (strlen(buffer) == 0) 
        isInt = false;

    // Kiểm tra số nhập vào có phải là số âm hay không     
    // Kiểm tra tràn số
    if (buffer[0] == '-')
    {
        // Nếu là số âm:
        isNegative = true;
        i = 1;
        
        if (strlen(buffer) > 11)
        // Tràn số về số ký tự
            isInt = false;

        // Tràn số về giá trị 
        if (strlen(buffer) == 11) {
            if (strcmp(buffer, "-2147483648") > 0)
            // Giá trị bị vượt biên -> không phải int
                isInt = false;
            
            if (strcmp(buffer, "-2147483648") == 0)
            // Kết quả ngay tại biên
                result = INT_MIN;
        }
    }
    else {
        // Không phải là số âm:
        isNegative = false;
        i = 0;

        if (strlen(buffer) > 10) 
        // Tràn số về số ký tự
            isInt = false;

        // Tràn số về giá trị
        if (strlen(buffer) == 10) 
            if (strcmp(buffer, "2147483647") > 0) 
            // Giá trị vượt biên -> không phải int
                isInt = false;
    }
    
    // Kiểm tra các kí tự nhập vào có phải số hay không
    if (isInt) 
        while (buffer[i] != '\0') {
            if (buffer[i] < 48 || buffer[i] > 57) {
                // Không phải là ký tự số
                isInt = false;
                break;
            }
            i++;
        }

    int num = 0;
    // chuyển chuỗi thành số
    // Nếu là số (isInt == True)
    if (isInt) {
        if (result != INT_MIN) {
            i = 0;
            if (isNegative)
            // Nếu là số âm thì index = 1 (vị trí bắt đầu số)
                i = 1;
            
            // Đọc số từ buffer
            while (buffer[i] != '\0') {
                num = num * 10 + int(buffer[i]) - '0';
                i++;
            }
            
            // Trả về kết quả (là trị tuyệt đối)
            if (isNegative) {
                num = -num;
            }
            result = num;
        }
    }
    else
        result = 0;

    delete[] buffer;
    // Đưa kết quả vào reg2
    kernel->machine->WriteRegister(2, result);
}

void SysPrintNum()
{
    /* 
        Input   : Số nguyên Int
        Output  : NULL
        Used    : In một số nguyên ra màn hình
    */
    // Kiểm tra số -
    bool isNegative = false;
    // Đọc number từ reg4
    int number = kernel->machine->ReadRegister(4);
    // Vị trí (index) 
    int i = 0;
    // Chuỗi ký tự (buffer)
    char *buffer = new char[12 + 1];

    // Kiểm tra number có = 0 hay không 
    // Nếu number != 0:
    if (number == INT_MIN) {
        strcpy(buffer, "-2147483648");
        buffer[strlen(buffer)] = '\0';
        i = 0;
        while (buffer[i] != '\0') {
            kernel->synchConsoleOut->PutChar(buffer[i++]);
        }
    }
    else {
        if (number != 0) {
            if (number < 0) {
                isNegative = true;
                number = -number;
            }
            int num = 0;
            while (number != 0) {
                num = number % 10;
                buffer[i] = num + '0';
                number = number / 10;
                i++;
            }
            if (isNegative) {
                buffer[i] = '-';
                i++;
            }
        }
        else {
            buffer[i] = '0';
            i++;
        }
        buffer[i] = '\0';
        while (i >= 0) {
            kernel->synchConsoleOut->PutChar(buffer[i--]);
        }
    }
    delete[] buffer;
}

#endif /* ! __USERPROG_KSYSCALL_H__ */