#include "UI.h"
#include "../state.h"
#include "./memory/process.h"
#include "./memory/memory_read.h"

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

    MemoryReader::UpdateFrozenValues();

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



   // ... початок функції Render() та кнопка вибору процесу ...

    ImGui::Text("Selected Process: %s (PID: %lu)",
    UIState::selectedProcessName.c_str(),
    UIState::selectedProcessID);

    ImGui::Separator();
    ImGui::Spacing();

    // ==============================================================
    // ПОЧАТОК ПАНЕЛІ ВКЛАДОК
    // ==============================================================
    if (ImGui::BeginTabBar("MainTabBar")) {

        // ----------------------------------------------------------
        // ВКЛАДКА 1: SCANNER (Тут тепер лежить твій поточний код)
        // ----------------------------------------------------------
        if (ImGui::BeginTabItem("Scanner")) {

            ImGui::Spacing();

            // Твої поля вводу
            ImGui::SetNextItemWidth(200.0f);
            ImGui::InputText("Value", UIState::searchValue, IM_ARRAYSIZE(UIState::searchValue));
            ImGui::SameLine();
            ImGui::SetNextItemWidth(180.0f);
            ImGui::Combo("Type", &UIState::selectedTypeIndex, UIState::dataTypes, IM_ARRAYSIZE(UIState::dataTypes));
            ImGui::Spacing();

            // Твої кнопки
            if (ImGui::Button("First Scan", ImVec2(120, 35))) {
                MemoryReader::PerformFirstScan();
            }
            ImGui::SameLine();
            if (ImGui::Button("Next Scan", ImVec2(120, 35))) {
                MemoryReader::PerformNextScan();
            }
            ImGui::Spacing();
            ImGui::Separator();

            // Твоя таблиця
            ImVec2 tableSize = ImVec2(0.0f, 0.0f);
         if (ImGui::BeginTable("ScanResultsTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY, ImVec2(0.0f, 0.0f))) {

                // Налаштовуємо колонки (робимо колонку Freeze вузькою)
                ImGui::TableSetupColumn("Freeze", ImGuiTableColumnFlags_WidthFixed, 50.0f);
                ImGui::TableSetupColumn("Address");
                ImGui::TableSetupColumn("Value");
                ImGui::TableSetupColumn("Previous");
                ImGui::TableHeadersRow();

                // ВАЖЛИВО: використовуємо auto& (з амперсандом), щоб змінювати реальні дані, а не копію
                for (auto& result : UIState::scanResults) {
                    ImGui::PushID(result.address);
                    ImGui::TableNextRow();

                    // Колонка 0: Чекбокс заморозки
                    ImGui::TableSetColumnIndex(0);
                    // "##" ховає текст ярлика, залишаючи лише саму галочку
                    ImGui::Checkbox("##freeze", &result.isFrozen);

                    // Колонка 1: Адреса (з контекстним меню)
                    ImGui::TableSetColumnIndex(1);
                    char addressText[32];
                    snprintf(addressText, sizeof(addressText), "%08X", result.address);
                    ImGui::Text("%s", addressText);

                    if (ImGui::BeginPopupContextItem("RowContextMenu")) {
                        if (ImGui::Selectable("Copy Address")) ImGui::SetClipboardText(addressText);
                        if (ImGui::Selectable("Copy Value")) ImGui::SetClipboardText(result.currentValue.c_str());
                        ImGui::EndPopup();
                    }

                    // Колонка 2: Поточне значення
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%s", result.currentValue.c_str());

                    // Колонка 3: Попереднє значення
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%s", result.previousValue.c_str());

                    ImGui::PopID();
                }
                ImGui::EndTable();
            }

            // ЗАКРИВАЄМО ВКАДКУ SCANNER
            ImGui::EndTabItem();
        }

        // ==========================================
        // ВКЛАДКА 2: WRITE
        // ==========================================
        if (ImGui::BeginTabItem("Write")) {
            static char writeAddress[64] = "";
            static char writeValue[64] = "";

            ImGui::Text("Manual Memory Write");
            ImGui::Spacing();

            ImGui::InputText("Address (HEX)", writeAddress, IM_ARRAYSIZE(writeAddress));
            ImGui::InputText("Value", writeValue, IM_ARRAYSIZE(writeValue));

            if (ImGui::Button("Write to Memory", ImVec2(150, 30))) {
                try {

                    uintptr_t addr = std::stoull(writeAddress, nullptr, 16);

                    // Перетворюємо введений текст значення на число int
                    int val = std::stoi(writeValue);

                    // Викликаємо нашу функцію бекенду
                    MemoryReader::WriteValue(addr, val);

                } catch (...) {

                }
            }

            ImGui::EndTabItem();
        }




        ImGui::EndTabBar();
    }

    ImGui::End();


    //  Вікно списку процесів ------------------------------------------------
    if (UIState::showProcessWindow) {
        ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("Process List", &UIState::showProcessWindow)) {
            // Створюємо фільтр (static, щоб він запам'ятовував введений текст між кадрами)
            static ImGuiTextFilter processFilter;

            // Малюємо поле вводу (зверху списку)
            processFilter.Draw("Search");

            ImGui::Separator();

            static std::vector<ProcessInfo> processes = GetSortedProcesses();

            if (ImGui::Button("Refresh List")) {
                processes = GetSortedProcesses();
            }

            ImGui::Separator();

            if (ImGui::BeginChild("ProcessScrollArea", ImVec2(0, 0), true)) {
                for (const auto& proc : processes) {
                    // Конвертуємо wstring у char масив (UTF-8)
                    char buf[256];
                    WideCharToMultiByte(CP_UTF8, 0, proc.name.c_str(), -1, buf, sizeof(buf), nullptr, nullptr);

                    // --- САМЕ ТУТ ПРАЦЮЄ ФІЛЬТР ---
                    // Якщо назва процесу (buf) не містить введеного тексту, переходимо до наступного
                    if (!processFilter.PassFilter(buf)) {
                        continue;
                    }
                    // ------------------------------

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