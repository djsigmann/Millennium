﻿#include <core/loader.hpp>
#include <filesystem>
#include <fstream>
#include <deps/deps.h>

#include <stdexcept>
#include <exception>
#include <typeinfo>
#include <fmt/core.h>
#ifdef _WIN32
#include <_win32/thread.h>
#include <_win32/cmd.h>
#endif

const bool __attribute__((constructor)) setup_env() 
{
#ifdef _WIN32
    if (steam::get().params.has("-dev") && static_cast<bool>(AllocConsole())) {
        void(freopen("CONOUT$", "w", stdout));
        void(freopen("CONOUT$", "w", stderr));
    }
#endif

#ifdef _WIN32
    constexpr const char* filePath = ".cef-enable-remote-debugging";
#elif __linux__
    std::string filePath = fmt::format("{}/.local/share/Steam/.cef-enable-remote-debugging", std::getenv("HOME"));
#endif

    if (!std::filesystem::exists(filePath)) {
        std::ofstream(filePath).close();
        return false;
    }
    return true;
}

const void bootstrap()
{
    if (!dependencies::clone_millennium_module()) {
        return;
    }
	plugin::get().bootstrap();
}

#ifdef _WIN32
int __attribute__((__stdcall__)) DllMain(void*, unsigned long fdwReason, void*) 
{
    if (fdwReason == DLL_PROCESS_ATTACH) {
        std::thread(bootstrap).detach();
    }
    return true;
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    bootstrap();
    return 1;
}
#elif __linux__
int main()
{
    bootstrap();
    return 1;
}
#endif