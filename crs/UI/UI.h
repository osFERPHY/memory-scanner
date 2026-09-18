#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <windows.h>

namespace UI {
    void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
    void NewFrame();
    void Render();
    void EndFrame();
    void Shutdown();
}