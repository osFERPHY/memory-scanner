#pragma once

#include <windows.h>
#include <string>

namespace UIState {
  //вікно для процесів
    inline bool showProcessWindow = false;


    // айді процесу який ми вибрали в вікні для процесів
    inline DWORD selectedProcessID = 0;
    inline std::string selectedProcessName = "No Process Selected";

}