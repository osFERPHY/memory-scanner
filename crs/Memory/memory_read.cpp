#include "memory_read.h"
#include "../state.h"
#include <iostream>

namespace MemoryReader {
    HANDLE OpenTargetProcess(DWORD processID) {
        if (processID == 0) {
            return nullptr;
        }

        // Запитуємо права на читання пам'яті
        HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, processID);

        if (!hProcess) {
            // Якщо не вдалося відкрити
            return nullptr;
        }

        return hProcess;
    }
    void PerformFirstScan() {
        if (UIState::selectedProcessID == 0) return;

        // Поки що реалізуємо тільки для 4 Bytes (Int32) — індекс 0
        if (UIState::selectedTypeIndex != 0) {
            return; // Інші типи додамо пізніше
        }

        int targetValue = 0;
        try {
            // Перетворюємо текст із поля вводу на число int
            targetValue = std::stoi(UIState::searchValue);
        } catch (...) {
            return; // Якщо користувач ввів літери замість чисел, скасування
        }

        HANDLE hProcess = OpenTargetProcess(UIState::selectedProcessID);
        if (!hProcess) return;

        // Очищаємо попередні результати
        UIState::scanResults.clear();

        // Отримуємо мінімальну та максимальну адресу пам'яті
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        uintptr_t currentAddress = (uintptr_t)sysInfo.lpMinimumApplicationAddress;
        uintptr_t maxAddress = (uintptr_t)sysInfo.lpMaximumApplicationAddress;

        MEMORY_BASIC_INFORMATION mbi;

        // Перебираємо всі регіони пам'яті
        while (currentAddress < maxAddress) {
            if (VirtualQueryEx(hProcess, (LPCVOID)currentAddress, &mbi, sizeof(mbi)) == 0) {
                break;
            }

            // Перевіряємо, чи пам'ять виділена
            if (mbi.State == MEM_COMMIT &&
               (mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_READONLY ||
                mbi.Protect == PAGE_EXECUTE_READ || mbi.Protect == PAGE_EXECUTE_READWRITE)) {

                // Створюємо буфер розміром з поточний регіон
                std::vector<char> buffer(mbi.RegionSize);
                SIZE_T bytesRead = 0;

                if (ReadProcessMemory(hProcess, mbi.BaseAddress, buffer.data(), mbi.RegionSize, &bytesRead)) {


                    for (size_t i = 0; i <= bytesRead - sizeof(int); i++) {
                        int* valPtr = (int*)(buffer.data() + i);

                        if (*valPtr == targetValue) {

                            ScanResult res;
                            res.address = (uintptr_t)mbi.BaseAddress + i;
                            res.currentValue = std::to_string(targetValue);
                            res.previousValue = "-";

                            UIState::scanResults.push_back(res);
                        }
                    }
                }
            }
            // Переходимо до наступного блоку пам'яті
            currentAddress += mbi.RegionSize;
        }

        CloseHandle(hProcess);
    }
    void PerformNextScan() {

        if (UIState::selectedProcessID == 0 || UIState::scanResults.empty()) return;


        if (UIState::selectedTypeIndex != 0) {
            return;
        }

        int targetValue = 0;
        try {

            targetValue = std::stoi(UIState::searchValue);
        } catch (...) {
            return;
        }

        HANDLE hProcess = OpenTargetProcess(UIState::selectedProcessID);
        if (!hProcess) return;

        // Створюємо новий список для збереження відфільтрованих результатів
        std::vector<ScanResult> filteredResults;

        // Перебираємо всі збережені раніше адреси
        for (const auto& oldResult : UIState::scanResults) {
            int currentValue = 0;
            SIZE_T bytesRead = 0;

            // Читаємо пам'ять за старою адресою
            if (ReadProcessMemory(hProcess, (LPCVOID)oldResult.address, &currentValue, sizeof(int), &bytesRead)) {


                if (currentValue == targetValue) {
                    ScanResult updatedResult = oldResult;


                    updatedResult.previousValue = oldResult.currentValue;

                    updatedResult.currentValue = std::to_string(currentValue);

                    filteredResults.push_back(updatedResult);
                }
            }
        }


        UIState::scanResults = filteredResults;

        CloseHandle(hProcess);
    }


    void WriteValue(uintptr_t address, int newValue) {
        if (UIState::selectedProcessID == 0) return;

        HANDLE hProcess = OpenTargetProcess(UIState::selectedProcessID);
        if (!hProcess) return;

        SIZE_T bytesWritten = 0;

        // Записуємо нове значення (4 байти для int) за вказаною адресою
        WriteProcessMemory(hProcess, (LPVOID)address, &newValue, sizeof(int), &bytesWritten);

        CloseHandle(hProcess);
    }

    
    void UpdateFrozenValues() {
        if (UIState::selectedProcessID == 0 || UIState::scanResults.empty()) return;

        // Спочатку перевіримо, чи є хоча б одна заморожена адреса, щоб не відкривати процес дарма
        bool hasFrozen = false;
        for (const auto& res : UIState::scanResults) {
            if (res.isFrozen) {
                hasFrozen = true;
                break;
            }
        }

        if (!hasFrozen) return;

        HANDLE hProcess = OpenTargetProcess(UIState::selectedProcessID);
        if (!hProcess) return;

        // Проходимось по всіх результатах і записуємо значення туди, де стоїть галочка
        for (const auto& res : UIState::scanResults) {
            if (res.isFrozen) {
                try {
                    int valueToWrite = std::stoi(res.currentValue);
                    SIZE_T bytesWritten = 0;
                    WriteProcessMemory(hProcess, (LPVOID)res.address, &valueToWrite, sizeof(int), &bytesWritten);
                } catch (...) {
                    // Ігноруємо, якщо значення неможливо конвертувати в число
                }
            }
        }

        CloseHandle(hProcess);
    }
}