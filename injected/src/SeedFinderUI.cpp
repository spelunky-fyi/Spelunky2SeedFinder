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
    bool SeedFinderUI::msHideUI = false;

    void SeedFinderUI::initialize(SeedFinder* seedFinder)
    {
        msSeedFinder = seedFinder;
        register_on_input(&processKeys);
        register_imgui_init(&initImGui);
        register_imgui_draw(&draw);
        register_post_draw(&postDraw);
    }

    void SeedFinderUI::initImGui(struct ImGuiContext*)
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
        if (nCode != WM_KEYDOWN || ImGui::GetIO().WantCaptureKeyboard)
        {
            return false;
        }

        if (wParam == 0x7a) // F11
        {
            msHideUI = !msHideUI;
        }

        return true;
    }

    void SeedFinderUI::draw()
    {
        if (!msHideUI)
        {
            ImGui::SetNextWindowPos({0, 0}, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize({600, ImGui::GetIO().DisplaySize.y / 2}, ImGuiCond_FirstUseEver);
            ImGui::Begin(msWindowTitle.c_str());
            msSeedFinder->render();
            ImGui::End();
        }

        auto& io = ImGui::GetIO();
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::SetNextWindowPos({0, 0});
        ImGui::Begin("Clickhandler", NULL,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
                         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNavInputs |
                         ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

        ImGui::InvisibleButton("canvas", ImGui::GetContentRegionMax(), ImGuiButtonFlags_MouseButtonRight);

        if (ImGui::IsMouseClicked(1) && ImGui::IsWindowFocused())
        {
            auto state = State::get();
            auto player = state.items()->player(0);
            if (player != nullptr)
            {
                ImVec2 mpos = normalize(io.MousePos);
                const auto [clickX, clickY] = state.click_position(mpos.x, mpos.y);
                player->teleport_abs(clickX, clickY, 0, 0);
            }
        }
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

    ImVec2 SeedFinderUI::normalize(ImVec2 pos)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 res = io.DisplaySize;
        if (res.x / res.y > 1.78)
        {
            pos.x -= (res.x - res.y / 9 * 16) / 2;
            res.x = res.y / 9 * 16;
        }
        else if (res.x / res.y < 1.77)
        {
            pos.y -= (res.y - res.x / 16 * 9) / 2;
            res.y = res.x / 16 * 9;
        }
        ImVec2 normal = ImVec2((pos.x - res.x / 2) * (1.0 / (res.x / 2)), -(pos.y - res.y / 2) * (1.0 / (res.y / 2)));
        return normal;
    }
} // namespace SeedFinder