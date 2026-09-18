#include "UI.h"

namespace UI {

enum class Tab { Aimbot, Wallhack, Triggerbot, Settings };
static Tab g_tab = Tab::Aimbot;

static void ApplyStyle() {
    ImGuiStyle& s = ImGui::GetStyle();
    s.WindowRounding   = 8.0f;
    s.FrameRounding    = 4.0f;
    s.TabRounding      = 4.0f;
    s.WindowBorderSize = 1.0f;
    s.WindowPadding    = { 12, 12 };
    s.FramePadding     = {  8,  5 };
    s.ItemSpacing      = {  8,  6 };

    ImVec4* c = s.Colors;
    c[ImGuiCol_WindowBg]       = { 0.08f, 0.08f, 0.10f, 1.0f };
    c[ImGuiCol_ChildBg]        = { 0.11f, 0.11f, 0.14f, 1.0f };
    c[ImGuiCol_Border]         = { 0.20f, 0.20f, 0.25f, 1.0f };
    c[ImGuiCol_FrameBg]        = { 0.15f, 0.15f, 0.18f, 1.0f };
    c[ImGuiCol_FrameBgHovered] = { 0.18f, 0.18f, 0.22f, 1.0f };
    c[ImGuiCol_Button]         = { 0.15f, 0.15f, 0.20f, 1.0f };
    c[ImGuiCol_ButtonHovered]  = { 0.20f, 0.60f, 1.00f, 0.3f };
    c[ImGuiCol_ButtonActive]   = { 0.20f, 0.60f, 1.00f, 1.0f };
    c[ImGuiCol_CheckMark]      = { 0.20f, 0.60f, 1.00f, 1.0f };
    c[ImGuiCol_SliderGrab]     = { 0.20f, 0.60f, 1.00f, 1.0f };
    c[ImGuiCol_Header]         = { 0.20f, 0.60f, 1.00f, 0.3f };
    c[ImGuiCol_HeaderHovered]  = { 0.20f, 0.60f, 1.00f, 0.5f };
    c[ImGuiCol_HeaderActive]   = { 0.20f, 0.60f, 1.00f, 1.0f };
    c[ImGuiCol_TitleBg]        = { 0.08f, 0.08f, 0.10f, 1.0f };
    c[ImGuiCol_TitleBgActive]  = { 0.08f, 0.08f, 0.10f, 1.0f };
    c[ImGuiCol_Text]           = { 0.90f, 0.90f, 0.95f, 1.0f };
    c[ImGuiCol_TextDisabled]   = { 0.50f, 0.50f, 0.55f, 1.0f };
    c[ImGuiCol_Separator]      = { 0.20f, 0.20f, 0.25f, 1.0f };
}

static void TabButton(const char* label, Tab id) {
    bool active = (g_tab == id);
    if (active) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.60f, 1.0f, 1.0f));
    else        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.20f, 1.0f));
    if (ImGui::Button(label, { 120.0f, 30.0f })) g_tab = id;
    ImGui::PopStyleColor();
    ImGui::SameLine();
}

static void RenderAimbot() {
    ImGui::TextDisabled("// Aimbot settings");
}
static void RenderWallhack() {
    ImGui::TextDisabled("// Wallhack settings");
}
static void RenderTriggerbot() {
    ImGui::TextDisabled("// Triggerbot settings");
}
static void RenderSettings() {
    ImGui::TextDisabled("// Settings");
}

void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;
    ApplyStyle();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, context);
}

void NewFrame() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void Render() {
    ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    ImGui::Begin("memory scanner", nullptr,
    ImGuiWindowFlags_NoCollapse    |
    ImGuiWindowFlags_NoScrollbar   |
    ImGuiWindowFlags_NoMove        |
    ImGuiWindowFlags_NoResize      |
    ImGuiWindowFlags_NoBringToFrontOnFocus);






    ImGui::End();
}

void EndFrame() {
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Shutdown() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

}