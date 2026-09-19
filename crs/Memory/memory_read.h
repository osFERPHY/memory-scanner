#pragma once
#include <windows.h>
#include <vector>
#include <string>

namespace MemoryReader {

    // Читання будь-якого значення за конкретною адресою
    template <typename T>
    bool ReadMemory(HANDLE hProcess, uintptr_t address, T& outValue) {
        SIZE_T bytesRead = 0;
        BOOL result = ReadProcessMemory(
            hProcess,
            reinterpret_cast<LPCVOID>(address),
            &outValue,
            sizeof(T),
            &bytesRead
        );
        return (result && bytesRead == sizeof(T));
    }

    // Відкриття процесу PID
    HANDLE OpenTargetProcess(DWORD processID);



    void PerformFirstScan();
    void PerformNextScan();
    void WriteValue(uintptr_t address, int newValue);
    void UpdateFrozenValues();
}