#pragma once

#include <ShlObj.h>
#include <Shlwapi.h>
#include <chrono>
#include <imgui.h>
#include <imgui_internal.h>
#include <string>
#include <unordered_map>

namespace SeedFinder
{
    class SeedFinder;

    class SeedFinderUI
    {
      public:
        static void initialize(SeedFinder* seedFinder);

      private:
        static bool processKeys(UINT nCode, WPARAM wParam, LPARAM lParam);
        static void initImGui(struct ImGuiContext*);
        static void draw();
        static void postDraw();
        static void mouseActivity();

        static SeedFinder* msSeedFinder;
        static std::string msWindowTitle;
        static ImFont* msFont;
        static ImFont* msBigFont;
        static std::chrono::system_clock::time_point msMouseMovedTime;
        static ImVec2 msMouseLastPosition;
        static bool msHideUI;
        static ImVec2 normalize(ImVec2 pos);
    };
} // namespace SeedFinder