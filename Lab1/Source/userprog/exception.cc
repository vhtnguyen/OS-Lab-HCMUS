/////////////////////////////////////////////////
// 	DH KHTN - DHQG TPHCM						/
// 	20120307		Phạm Gia Khiêm				/
// 	20120519		Nguyễn Thị Thúy Liễu		/
// 	20120540		Võ Hoàng Thảo Nguyên		/
/////////////////////////////////////////////////

// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "ksyscall.h"
#include "main.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which) {
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
    case NoException:
        return;

    case PageFaultException:
        /* A page fault occurs when a program attempts to access
        data or code that is in its address space,
        but is not currently located in the system RAM
        */
        DEBUG(dbgSys, "Invalid page found.\n");
        printf("\n\n Invalid page found");
        SysHalt();
        break;

    case ReadOnlyException:
        /* Read-only is a file attribute which only allows a user to view a file,
        restricting any writing to the file. */
        DEBUG(dbgSys, "Restrict any writing to the read-only page.\n");
        printf("\n\n Restrict any writing to the read-only page.");
        SysHalt();
        break;

    case BusErrorException:
        /*  bus error is a fault raised by hardware, notifying an operating system (OS)
        that a process is trying to access memory that the CPU cannot physically address:
        an invalid address for the address bus, hence the name.
        */
        DEBUG(dbgSys, "Translation resulted invalid physical address.\n");
        printf("\n\n Translation resulted invalid physical address");
        SysHalt();
        break;

    case AddressErrorException:
        /* An addressing error is detected by determining whether
        the target address output on the address path is detected at the memory.
        */
        DEBUG(dbgSys, "Unaligned reference or one that was beyond the end of the address space.\n");
        printf("\n\n Unaligned reference or one that was beyond the end of the address space");
        SysHalt();
        break;

    case OverflowException:
        DEBUG(dbgSys, "Overflow in add or sub type.\n");
        printf("\n\n Overflow in add or sub type.");
        SysHalt();
        break;

    case IllegalInstrException:
        // They arise because not every possible memory value corresponds to a valid instruction.
        DEBUG(dbgSys, "Unimplemented or reserved instruction.\n");
        printf("\n\n Unimplemented or reserved instruction.");
        SysHalt();
        break;

    case NumExceptionTypes:
        DEBUG(dbgSys, "Number exception types.\n");
        printf("\n\n Number exception types");
        SysHalt();
        break;

    case SyscallException: {
        switch (type) {
        case SC_Halt:
            /*
                Input	: NULL
                Output	: Notify for shut down
                Used	: Shut down Operating System
            */
            DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
            printf("\n\n Shutdown, initiated by user program.");

            SysHalt();
            ASSERTNOTREACHED();
            break;

        case SC_Add:
            /*
                Input	: r4, r5
                Output	: NULL
                Used	: Tính tổng giá trị của r4 và r5
            */
            break;

        case SC_Sub:
            /*
                Input	: r4 r5
                Output	: NULL
                Used	: Tính hiệu của giá trị r4 - r5
            */
            break;

        case SC_Create:
            /*
                Input	: Register 4 (chứa vùng nhớ của tên file)
                Output	: Thông báo kết quả tạo file (Lỗi = -1 | Thành công = 0)
                Used	: Tạo file với tên là filename
            */
            break;

        case SC_ReadChar: {
            /*
                Input	: NULL
                Output	: 1 ký tự (char)
                Used	: Đọc một ký tự từ người dùng nhập vào
            */
            SysReadChar();
            break;
        }

        case SC_PrintChar: {
            /*
                Input	: Ký tự (char)
                Output	: Ký tự (char)
                Used	: Xuất 1 ký tự ra màn hình
            */
            SysPrintChar();
            break;
        }
        case SC_ReadString: {
            /*
                Input	: NULL
                Output	: Chuỗi ký tự (char*)
                Used	: Đọc 1 chuỗi ký tự từ người dùng nhập vào
            */
            // lấy độ dài chuổi cần đọc tức là giá trị của arg 2
            int length = kernel->machine->ReadRegister(5);
            // printf("do dai chuoi vua nhap %d", length);
            if (length > MAXLENGTH || length < 1) {
                cerr << "\nDo dai chuoi khong hop le\n";
            }
            else {
                SysReadString(length);
            }
            break;
        }
        case SC_PrintString: {
            /*
                Input	: Chuỗi ký tự (char*)
                Output	: Chuỗi ký tự (char*)
                Used	: Xuất 1 chuỗi ký tự ra màn hình
            */
            SysPrintString();
            break;
        }
        case SC_RandomNum: {
            /*
                Input	: NULL
                Output	: Number ngẫu nhiên
                Used	: Xuất 1 số ngẫu nhiên ra màn hình
            */
            SysRandomNum();
            break;
        }
        case SC_ReadNum: {
            /*
                Input	: NULL
                Output	: Number
                Used	: Đọc 1 số từ người dùng nhập vào
            */
            SysReadNum();
            break;
        }
        case SC_PrintNum: {
            /*
                Input	: Number
                Output	: Number
                Used	: Xuất 1 số ra màn hình
            */
            SysPrintNum();
            break;
        }

        default:
            cerr << "Unexpected system call " << type << "\n";
            break;
        }
        // Tăng giá trị PC
        IncreasePC();
        break;
    }

    default:
        cerr << "Unexpected user mode exception" << (int)which << "\n";
        break;
    }
}