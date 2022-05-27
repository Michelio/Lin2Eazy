// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>
#include <stdio.h>
#include <Windows.h>

#define PIPE_RECEIVE_NAME L"\\\\.\\pipe\\PipeSend"
#define PIPE_SEND_NAME L"\\\\.\\pipe\\PipeReceive"
#define BUFSIZE 1 << 16

DWORD FirstParam;
HANDLE sendPipe;
HANDLE receivePipe;
void SendPacket(BYTE*,DWORD);
void(__cdecl* DataSend)(void* q, const char* format, ...);
DWORD(__stdcall* realDataRecv)(BYTE* packet, LPVOID trash, DWORD length);
DWORD(__stdcall myDataRecv)(BYTE* packet, LPVOID trash, DWORD length)
{
    DWORD result = realDataRecv(packet, trash, length);
    char* mPacket = new char[length + 1]();
    mPacket[0] = (char)1;

    for (int i = 1; i < length + 1; i++)
        mPacket[i] = packet[i - 1];

    SendPacket((BYTE*)mPacket, length + 1);
    printf("\n======  REAL PACKET  ======\n");

    printf("[%02X]\n", packet[0]);
    for (int i = 1; i < length; i++)
    { 
        if ((i - 1) % 4 == 0 && i != 1)
            printf("   ");

        printf("%02X ", packet[i]);
    }

    printf("\n======  REAL PACKET  ======\n\n");
    return result;
}

void SetupPipes()
{
    receivePipe = CreateFileW(PIPE_RECEIVE_NAME, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    sendPipe = CreateFileW(PIPE_SEND_NAME, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (receivePipe == INVALID_HANDLE_VALUE)
    {
        std::cout << "RPipe fail\n";
        return;
    }

    if (sendPipe == INVALID_HANDLE_VALUE)
    {
        std::cout << "SPipe fail\n";
        return;
    }
}

void SendPacket(BYTE* packet, DWORD length)
{
    DWORD write;
    printf("\n======  MODYFIED PACKET  ======\n");

    printf("[%02X]\n", packet[1]);
    for (int i = 2; i < length; i++)
    {
        if ((i - 2) % 4 == 0 && i != 2)
            printf("   ");

        printf("%02X ", packet[i]);
    }

    printf("\n======  MODYFIED PACKET  ======\n\n");

    if (sendPipe != INVALID_HANDLE_VALUE)
    {
        std::cout << "Sending target!\n";
        WriteFile(sendPipe,
            packet,
            length,
            &write,
            NULL);
    }
}

LPVOID Hook(LPVOID funcToHook, LPVOID myFunc, size_t size)
{
    DWORD old;
    LPVOID oldInstruction = malloc(5 + size);
    VirtualProtect(oldInstruction, size + 5, PAGE_EXECUTE_READWRITE, &old);
    memcpy(oldInstruction, funcToHook, size);
    *(BYTE*)((DWORD)oldInstruction + size) = 0xE9;
    *(DWORD*)((DWORD)oldInstruction + size + 1) = (DWORD)((DWORD)funcToHook + size) - (DWORD)((DWORD)oldInstruction + size) - 5;
    VirtualProtect(funcToHook, 5, PAGE_EXECUTE_READWRITE, &old);
    *(BYTE*)funcToHook = 0xE9;
    *(DWORD*)((DWORD)funcToHook + 1) = (DWORD)myFunc - (DWORD)funcToHook - 5;
    return oldInstruction;
}

void Send()
{
    int rand = 0;
    while (1)
    {
        Sleep(100);
        if (GetAsyncKeyState(VK_F2) & 1)
        {
            std::cout << "Sending target!\n";
            DataSend((LPVOID)FirstParam, "cddddc", 0x4, 0x100076CD, -85964, 240947, -3729, 0);
        }

        if (GetAsyncKeyState(VK_F3) & 1)
        {
            std::cout << "Sending msg!\n";
            std::string str = "spam" + std::to_string(rand++);
            std::cout << str << "\n";
            std::wstring widestr = std::wstring(str.begin(), str.end());
            const wchar_t* widecstr = widestr.c_str();
            DataSend((LPVOID)FirstParam, "cSd", 0x38, widecstr, 0);
        }
    }
}

BOOL WINAPI DllMain(HINSTANCE hInst,
    DWORD  reason,
    LPVOID q
)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);

        DWORD EngineBase = (DWORD)GetModuleHandleA("Engine.dll");
        FirstParam = EngineBase + 0x951044;
        FirstParam = *(DWORD*)FirstParam;
        FirstParam = *(DWORD*)(FirstParam + 0x60);
        FirstParam = *(DWORD*)(FirstParam + 0x48);
        std::cout << "Value " << std::hex << FirstParam << "\n";
        SetupPipes();
        DataSend = (void(__cdecl*)(void* q, const char* format, ...))(EngineBase + 0x1029B0);
        //CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Send, 0, 0, 0);
        realDataRecv = (DWORD(__stdcall*)(BYTE*, LPVOID, DWORD))Hook((LPVOID)(EngineBase + 0x102070), myDataRecv, 8);
    }
    return 1;
}

