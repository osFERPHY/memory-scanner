#include "Core/Application.h"

int main() {
    if (!App::Init()) return 1;
    App::Run();
    App::Shutdown();
    return 0;
}