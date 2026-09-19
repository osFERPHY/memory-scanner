#include "UI.h"
#include "../state.h"
#include "./memory/process.h"


namespace UI {






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




    void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    // Вмикаємо можливість виносити вікна за межі програми
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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


    //  main window тут ------------------------------------------------

    // координати вікон
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(viewport->WorkSize, ImGuiCond_Always);



    ImGui::Begin("memory scanner", nullptr,
        ImGuiWindowFlags_NoTitleBar    |
        ImGuiWindowFlags_NoCollapse    |
        ImGuiWindowFlags_NoScrollbar   |
        ImGuiWindowFlags_NoMove        |
        ImGuiWindowFlags_NoResize      |
        ImGuiWindowFlags_NoBringToFrontOnFocus);


//кнопка
    if (ImGui::Button("proces", ImVec2(90.0f, 40.0f))) {
        UIState::showProcessWindow = true;
    }
        ImGui::SetCursorPos(ImVec2(110, 25));
    ImGui::Text("select process: %s", UIState::selectedProcessName.c_str());





    ImGui::End();




    //  Вікно списку процесів ------------------------------------------------
    if (UIState::showProcessWindow) {
        ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("Process List", &UIState::showProcessWindow)) {


            static std::vector<ProcessInfo> processes = GetSortedProcesses();

            if (ImGui::Button("Refresh List")) {
                processes = GetSortedProcesses();
            }

            ImGui::Separator();

            if (ImGui::BeginChild("ProcessScrollArea", ImVec2(0, 0), true)) {
                for (const auto& proc : processes) {
                    char buf[256];
                    WideCharToMultiByte(CP_UTF8, 0, proc.name.c_str(), -1, buf, sizeof(buf), nullptr, nullptr);


                    char label[300];
                    snprintf(label, sizeof(label), "%s##%lu", buf, proc.processID);

                    if (ImGui::Selectable(label)) {

                        UIState::selectedProcessID = proc.processID;

                        UIState::selectedProcessName = buf;

                        UIState::showProcessWindow = false;


                        // logik memory 


                    }




                }
            }
            ImGui::EndChild();

        }
        ImGui::End();


    }
}
    //---------------------------- END ----------------------------------
    void EndFrame() {
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


    // Оновлюємо та малюємо відірвані вікна
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void Shutdown() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

}