#pragma once

#include <windows.h>
#include <vector>
#include <string>


struct ProcessInfo {
    DWORD processID;
    std::wstring name;
};


std::vector<ProcessInfo> GetSortedProcesses();