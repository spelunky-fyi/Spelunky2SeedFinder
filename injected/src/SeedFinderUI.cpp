// adapted from Overlunky injected/ui.cpp

#include "SeedFinderUI.h"
#include "SeedFinder.h"
#include "window_api.hpp"
#include <codecvt>
#include <sstream>


namespace SeedFinder
{
    SeedFinder* SeedFinderUI::msSeedFinder = nullptr;
    std::string SeedFinderUI::msWindowTitle = "Seed Finder";
    ImFont* SeedFinderUI::msFont = nullptr;
    ImFont* SeedFinderUI::msBigFont = nullptr;
    std::chrono::system_clock::time_point SeedFinderUI::msMouseMovedTime = std::chrono::system_clock::now();
    ImVec2 SeedFinderUI::msMouseLastPosition = ImVec2(0, 0);

    void SeedFinderUI::initialize(SeedFinder* seedFinder)
    {
        msSeedFinder = seedFinder;
        register_on_input(&processKeys);
        register_imgui_init(&initImGui);
        register_imgui_draw(&draw);
        register_post_draw(&postDraw);
    }

    void SeedFinderUI::initImGui()
    {
        ImGuiIO& io = ImGui::GetIO();
        io.FontAllowUserScaling = true;
        PWSTR fontdir;
        if (SHGetKnownFolderPath(FOLDERID_Fonts, 0, NULL, &fontdir) == S_OK)
        {
            using cvt_type = std::codecvt_utf8<wchar_t>;
            std::wstring_convert<cvt_type, wchar_t> cvt;

            std::string fontpath(cvt.to_bytes(fontdir) + "\\segoeuib.ttf");
            if (GetFileAttributesA(fontpath.c_str()) != INVALID_FILE_ATTRIBUTES)
            {
                msFont = io.Fonts->AddFontFromFileTTF(fontpath.c_str(), 18.0f);
                msBigFont = io.Fonts->AddFontFromFileTTF(fontpath.c_str(), 32.0f);
            }

            CoTaskMemFree(fontdir);
        }

        if (!msFont)
        {
            msFont = io.Fonts->AddFontDefault();
        }
    }

    bool SeedFinderUI::processKeys(UINT nCode, WPARAM wParam, LPARAM lParam)
    {
        if (nCode != WM_KEYDOWN)
        {
            return false;
        }
        return true;
    }

    void SeedFinderUI::draw()
    {
        ImGui::SetNextWindowPos({0, 0}, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize({600, ImGui::GetIO().DisplaySize.y / 2}, ImGuiCond_FirstUseEver);
        ImGui::Begin(msWindowTitle.c_str());
        msSeedFinder->render();
        ImGui::End();
    }

    void SeedFinderUI::postDraw()
    {
        mouseActivity();
    }

    void SeedFinderUI::mouseActivity()
    {
        using namespace std::chrono_literals;
        auto& io = ImGui::GetIO();
        if (io.MousePos.x != msMouseLastPosition.x || io.MousePos.y != msMouseLastPosition.y)
        {
            msMouseMovedTime = std::chrono::system_clock::now();
            io.MouseDrawCursor = true;
            msMouseLastPosition = io.MousePos;
        }
        else if (msMouseMovedTime + 2s < std::chrono::system_clock::now())
        {
            io.MouseDrawCursor = false;
        }
    }

} // namespace SeedFinder