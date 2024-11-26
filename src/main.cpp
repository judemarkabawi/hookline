#include <iostream>

#include "core/Application.hpp"

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;
    return main(__argc, __argv);
}
#endif

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    try {
        Application app;
        app.init();
        app.run();
    } catch (std::exception const& e) {
        std::cerr << "Unhandled exception:\n" << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unhandled exception (unknown type)." << std::endl;
        throw;
    }

    return 0;
}