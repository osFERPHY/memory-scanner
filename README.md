# Memory Scanner (WIP)

A work-in-progress memory scanning and debugging tool built for Windows. The project features a modern, detachable user interface and a multi-threaded architecture.

## 🚀 Features (Current & Planned)
* **Advanced UI:** Built with [Dear ImGui](https://github.com/ocornut/imgui), featuring Multi-Viewports for detachable, native OS windows.
* **Multi-Threaded Architecture:** Dedicated rendering thread separate from the Win32 message loop for smooth, lock-free performance (`std::recursive_mutex`).
* **Graphics Backend:** Hardware-accelerated rendering using DirectX 11.
* **Process Enumeration:** (In Progress) Fetching running system processes and windows.
* **Memory Scanning:** (Planned) Value searching, pointer scanning, and memory manipulation.

## 🛠️ Tech Stack
* **Language:** C++20
* **Build System:** CMake
* **Graphics API:** DirectX 11
* **UI Library:** Dear ImGui (v1.93.0 WIP)
* **OS API:** Win32 API (Toolhelp32)

## 🏗️ How to Build
1. Clone the repository.
2. Open the project in CLion (or your preferred IDE with CMake support).
3. The `CMakeLists.txt` will automatically link the required DirectX11 and Win32 libraries (`d3d11`, `d3dcompiler`, `dwmapi`).
4. Build and run the `memory_scanner` executable.

## 📌 Status
**Work In Progress.** The core rendering engine, UI layout, and Multi-Viewport system are fully functional. The backend logic for Windows process memory reading/writing is currently under active development.