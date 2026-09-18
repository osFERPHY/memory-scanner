#include "Process.h"
#include <tlhelp32.h>
#include <algorithm>

std::vector<ProcessInfo> GetSortedProcesses() {
    std::vector<ProcessInfo> processes;

    // Створюємо знімок усіх процесів у системі
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return processes;
    }

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    // Отримуємо перший процес
    if (Process32FirstW(hSnapshot, &pe32)) {
        do {
            processes.push_back({ pe32.th32ProcessID, pe32.szExeFile });
        } while (Process32NextW(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);

    // Сортуємо процеси від більшого до меншого за PID
    std::sort(processes.begin(), processes.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        return a.processID > b.processID;
    });

    return processes;
}