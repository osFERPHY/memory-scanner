#pragma once

#include <windows.h>
#include <string>
#include <vector>



struct ScanResult {
    uintptr_t address;          // Адреса в пам'яті
    std::string currentValue;   // Поточне значення
    std::string previousValue;  // Попереднє значення
    bool isFrozen = false;
};





namespace UIState {
    inline bool showProcessWindow = false;

    inline DWORD selectedProcessID = 0;
    inline std::string selectedProcessName = "No Process Selected";

    // --- Нові змінні для сканування ---
    inline char searchValue[128] = "100"; // Буфер для тексту введення
    inline int selectedTypeIndex = 0;     // Індекс обраного типу даних у списку


    inline std::vector<ScanResult> scanResults;





    // сканування
    inline const char* dataTypes[] = {
        "4 Bytes (Int32)",
        "2 Bytes (Int16)",
        "1 Byte (Int8)",
        "8 Bytes (Int64)",
        "Float",
        "Double"



    };
}