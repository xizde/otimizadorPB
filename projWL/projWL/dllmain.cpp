#include "pch.h"
#include <windows.h>
#include <iostream>

char fpsmaxativo[] = "\x90\x90\x90\x90\x90\x90\x90\x90";
char fpsmaxdesativo[] = "\xF2\x0F\x5E\x15\x08\xC7\x7A\x01";

void escrita(uintptr_t addressWrite, char* valueWrite, int bytenum) {
    DWORD oldprotect;
    VirtualProtect((LPVOID)(addressWrite), bytenum, PAGE_EXECUTE_READWRITE, &oldprotect);
    memcpy((LPVOID)addressWrite, valueWrite, bytenum);
    VirtualProtect((LPVOID)(addressWrite), bytenum, oldprotect, NULL);
}

DWORD WINAPI liberarfps(LPVOID lpParam) {
    uintptr_t dwGameBase = (uintptr_t)GetModuleHandleA("PointBlank.exe");
    if (dwGameBase == 0) return 0; // Proteção caso não encontre o jogo

    uintptr_t Address = dwGameBase + 0x1026C8;
    escrita(Address, fpsmaxativo, 8);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, liberarfps, NULL, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
