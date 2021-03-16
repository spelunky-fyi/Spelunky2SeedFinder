// Adapted from Overlunky injector/main.cpp

#include <chrono>
#include <filesystem>
#include <thread>

#include "injector.h"
#include "logger.h"

namespace fs = std::filesystem;
using namespace std::chrono_literals;

fs::path get_dll_path()
{
    char buf[0x1000];
    GetModuleFileNameA(NULL, buf, sizeof(buf));
    fs::path path(buf);
    return path.parent_path().concat("\\Spelunky2SeedFinder.dll");
}

int main(int argc, char** argv)
{

    auto pathToDLL = get_dll_path();

    if (!fs::exists(pathToDLL))
    {
        PANIC("DLL not found! {}", pathToDLL.string().data());
    }

    INFO("Spelunky2SeedFinder version: " TOSTRING(GIT_VERSION));

    INFO("Searching for Spel2.exe process...");
    bool started = true;
    Process gameProcess;
    while (true)
    {
        if (auto res = find_process("Spel2.exe"))
        {
            gameProcess = res.value();
            break;
        }
        started = false;
        std::this_thread::sleep_for(1s);
    };
    INFO("Found Spel2.exe PID: {}", gameProcess.info.pid);
    if (!started)
    {
        INFO("Game was just started, waiting a few seconds for it to "
             "load before injecting...");
        std::this_thread::sleep_for(1s);
    }

    inject_dll(gameProcess, pathToDLL.string());
    call(gameProcess, find_function(gameProcess, pathToDLL.string(), "run"), (LPVOID)(uint64_t)GetCurrentProcessId());
}
