// adapted from Overlunky injected/main.cpp

#include <Windows.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "SeedFinder.h"
#include "SeedFinderUI.h"
#include "logger.h"
#include "memory.h"
#include "render_api.hpp"
#include "state.hpp"
#include "window_api.hpp"


using namespace std::chrono_literals;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, // handle to DLL module
                    DWORD fdwReason,    // reason for calling function
                    LPVOID lpReserved)  // reserved
{
    return TRUE; // Successful DLL_PROCESS_ATTACH.
}

BOOL WINAPI ctrl_handler(DWORD ctrl_type)
{
    switch (ctrl_type)
    {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
    {
        DEBUG("Console detached, you can now close this window.");
        FreeConsole();
        return TRUE;
    }
    }
    return TRUE;
}

void attach_stdout(DWORD pid)
{
    AttachConsole(pid);
    SetConsoleCtrlHandler(ctrl_handler, 1);

    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);
}

extern "C" __declspec(dllexport) void run(DWORD pid)
{
    attach_stdout(pid);
    DEBUG("Game injected! Press Ctrl+C to detach this window from the process.");

    auto seedFinder = std::make_unique<SeedFinder::SeedFinder>();

    while (true)
    {
        auto entities = list_entities();
        if (entities.size() >= 850)
        {
            DEBUG("Found {} entities, that's enough", entities.size());
            std::this_thread::sleep_for(100ms);
            seedFinder->registerAllEntities(entities);
            DEBUG("Added {} entities", entities.size());
            break;
        }
        else if (entities.size() > 0)
        {
            DEBUG("Found {} entities", entities.size());
        }
        std::this_thread::sleep_for(100ms);
    }

    auto api = RenderAPI::get();
    SeedFinder::SeedFinderUI::initialize(seedFinder.get());
    init_hooks((void*)api.swap_chain());
    DEBUG("Running in debug mode.");
    do
    {
        std::string line;
        std::getline(std::cin, line);
        if (std::cin.fail() || std::cin.eof())
        {
            std::cin.clear();
        }
    } while (true);
}
