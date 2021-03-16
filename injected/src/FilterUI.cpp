#include "FilterUI.h"
#include "Filter.h"
#include <fmt/format.h>
#include <iostream>

namespace SeedFinder
{
    void FilterUI::renderLevelSelectorCheckboxes(LevelStorage* levelsToSearch, bool showAllNoneButtons)
    {
        const auto renderCheckbox = [levelsToSearch](bool* valueHolder, uint8_t world, uint8_t level, const std::string& caption, bool sameLine = true) {
            bool disabledDummy = false;
            if (!levelsToSearch->isHidden(world, level))
            {
                if (sameLine)
                {
                    ImGui::SameLine();
                }
                if (levelsToSearch->isDisabled(world, level))
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(50, 50, 50)));
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(ImColor(50, 50, 50)));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(ImColor(50, 50, 50)));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(ImColor(50, 50, 50)));
                    ImGui::Checkbox(fmt::format("{}##SeedFinderLevel{}{}", caption, caption, fmt::ptr(levelsToSearch)).c_str(), &disabledDummy);
                    ImGui::PopStyleColor(4);
                }
                else
                {
                    ImGui::Checkbox(fmt::format("{}##SeedFinderLevel{}{}", caption, caption, fmt::ptr(levelsToSearch)).c_str(), valueHolder);
                }
            }
        };

        ImGui::SetCursorPosX(gSectionMarginHor * 2);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Level :");
        ImGui::SameLine();
        auto left = ImGui::GetCursorPosX();
        renderCheckbox(&levelsToSearch->OneOne, 1, 1, "1-1", false);
        renderCheckbox(&levelsToSearch->OneTwo, 1, 2, "1-2");
        renderCheckbox(&levelsToSearch->OneThree, 1, 3, "1-3");
        renderCheckbox(&levelsToSearch->Quillback, 1, 4, "Quillback");
        ImGui::SetCursorPosX(left);
        renderCheckbox(&levelsToSearch->TwoOne, 2, 1, "2-1", false);
        renderCheckbox(&levelsToSearch->TwoTwo, 2, 2, "2-2");
        renderCheckbox(&levelsToSearch->TwoThree, 2, 3, "2-3");
        renderCheckbox(&levelsToSearch->TwoFour, 2, 4, "2-4");
        renderCheckbox(&levelsToSearch->Olmec, 3, 1, "Olmec");
        ImGui::SetCursorPosX(left);
        renderCheckbox(&levelsToSearch->FourOne, 4, 1, "4-1", false);
        renderCheckbox(&levelsToSearch->FourTwo, 4, 2, "4-2");
        renderCheckbox(&levelsToSearch->FourThree, 4, 3, "4-3");
        renderCheckbox(&levelsToSearch->FourFour, 4, 4, "4-4");
        renderCheckbox(&levelsToSearch->IceCaves, 5, 1, "Ice Caves");
        ImGui::SetCursorPosX(left);
        renderCheckbox(&levelsToSearch->SixOne, 6, 1, "6-1", false);
        renderCheckbox(&levelsToSearch->SixTwo, 6, 2, "6-2");
        renderCheckbox(&levelsToSearch->SixThree, 6, 3, "6-3");
        renderCheckbox(&levelsToSearch->Tiamat, 6, 4, "Tiamat");
        ImGui::SetCursorPosX(left);
        renderCheckbox(&levelsToSearch->SevenOne, 7, 1, "7-1", false);
        renderCheckbox(&levelsToSearch->SevenTwo, 7, 2, "7-2");
        renderCheckbox(&levelsToSearch->SevenThree, 7, 3, "7-3");
        renderCheckbox(&levelsToSearch->Hundun, 7, 4, "Hundun");

        if (showAllNoneButtons)
        {
            ImGui::SameLine();
            if (ImGui::Button(fmt::format("All##SeedFinderLevelAll{}", fmt::ptr(levelsToSearch)).c_str()))
            {
                levelsToSearch->all();
            }
            ImGui::SameLine();
            if (ImGui::Button(fmt::format("None##SeedFinderLevelNone{}", fmt::ptr(levelsToSearch)).c_str()))
            {
                levelsToSearch->none();
            }
        }
    }

    void FilterUI::renderLevelSelectorRadiobuttons(LevelStorage* levelsToSearch, bool showAnyButton)
    {
        const auto renderRadiobutton = [levelsToSearch](uint8_t levelDepth, uint8_t world, uint8_t level, const std::string& caption, bool sameLine = true) {
            if (!levelsToSearch->isHidden(world, level))
            {
                if (sameLine)
                {
                    ImGui::SameLine();
                }
                if (levelsToSearch->isDisabled(world, level))
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(50, 50, 50)));
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(ImColor(50, 50, 50)));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(ImColor(50, 50, 50)));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(ImColor(50, 50, 50)));
                    if (ImGui::RadioButton(fmt::format("{}##SeedFinderLevel{}{}", caption, caption, fmt::ptr(levelsToSearch)).c_str(), false))
                    {
                        // noop
                    }
                    ImGui::PopStyleColor(4);
                }
                else
                {
                    if (ImGui::RadioButton(fmt::format("{}##SeedFinderLevel{}{}", caption, caption, fmt::ptr(levelsToSearch)).c_str(), levelsToSearch->deepest() == levelDepth))
                    {
                        levelsToSearch->none();
                        levelsToSearch->activateDepth(levelDepth);
                    }
                }
            }
        };

        ImGui::SetCursorPosX(gSectionMarginHor * 2);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Level :");
        ImGui::SameLine();
        auto left = ImGui::GetCursorPosX();
        renderRadiobutton(0, 1, 1, "1-1", false);
        renderRadiobutton(1, 1, 2, "1-2");
        renderRadiobutton(2, 1, 3, "1-3");
        renderRadiobutton(3, 1, 4, "Quillback");
        ImGui::SetCursorPosX(left);
        renderRadiobutton(4, 2, 1, "2-1", false);
        renderRadiobutton(5, 2, 2, "2-2");
        renderRadiobutton(6, 2, 3, "2-3");
        renderRadiobutton(7, 2, 4, "2-4");
        renderRadiobutton(8, 3, 1, "Olmec");
        ImGui::SetCursorPosX(left);
        renderRadiobutton(9, 4, 1, "4-1", false);
        renderRadiobutton(10, 4, 2, "4-2");
        renderRadiobutton(11, 4, 3, "4-3");
        renderRadiobutton(12, 4, 4, "4-4");
        renderRadiobutton(13, 5, 1, "Ice Caves");
        ImGui::SetCursorPosX(left);
        renderRadiobutton(14, 6, 1, "6-1", false);
        renderRadiobutton(15, 6, 2, "6-2");
        renderRadiobutton(16, 6, 3, "6-3");
        renderRadiobutton(17, 6, 4, "Tiamat");
        ImGui::SetCursorPosX(left);
        renderRadiobutton(18, 7, 1, "7-1", false);
        renderRadiobutton(19, 7, 2, "7-2");
        renderRadiobutton(20, 7, 3, "7-3");
        renderRadiobutton(21, 7, 4, "Hundun");
        if (showAnyButton)
        {
            renderRadiobutton(99, 99, 99, "Any");
        }
    }

    void FilterUI::renderLayerSelector(LayerChoice* layer, Filter* filter)
    {
        static const char* layerOptions[] = {"Any", "Front", "Back"};
        const char* chosenLayer = layerOptions[0];
        if (*layer == LayerChoice::FRONT)
        {
            chosenLayer = layerOptions[1];
        }
        else if (*layer == LayerChoice::BACK)
        {
            chosenLayer = layerOptions[2];
        }

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Layer :");
        ImGui::SameLine();
        ImGui::PushItemWidth(100);
        if (ImGui::BeginCombo(fmt::format("##SeedFinderLayer{}", fmt::ptr(filter)).c_str(), chosenLayer))
        {
            for (auto i = 0; i < IM_ARRAYSIZE(layerOptions); ++i)
            {
                bool isSelected = (chosenLayer == layerOptions[i]);
                if (ImGui::Selectable(layerOptions[i], isSelected))
                {
                    switch (i)
                    {
                    case 0:
                        *layer = LayerChoice::ALL;
                        break;
                    case 1:
                        *layer = LayerChoice::FRONT;
                        break;
                    case 2:
                        *layer = LayerChoice::BACK;
                        break;
                    }
                }
            }
            ImGui::EndCombo();
        }
    }

    void FilterUI::renderDeleteFilterButton(Filter* filter)
    {
        static const uint8_t closeButtonWidth = 25;
        static const uint8_t closeButtonMarginRight = 15;
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - closeButtonWidth - closeButtonMarginRight);
        if (ImGui::Button(fmt::format("X##{}", fmt::ptr(filter)).c_str()))
        {
            filter->markAsDeleted();
        }
    }

    void FilterUI::renderAccessibilityChoice(AccessibilityChoice* accessibility, Filter* filter, const char* prefix, const char* suffix)
    {
        static const char* accessibilityOptions[] = {"may or may not", "must", "must not"};
        const char* chosenAccessibility = accessibilityOptions[0];
        if (*accessibility == AccessibilityChoice::YES)
        {
            chosenAccessibility = accessibilityOptions[1];
        }
        else if (*accessibility == AccessibilityChoice::NO)
        {
            chosenAccessibility = accessibilityOptions[2];
        }

        ImGui::AlignTextToFramePadding();
        ImGui::Text(prefix);
        ImGui::SameLine();

        ImGui::PushItemWidth(145);
        if (ImGui::BeginCombo(fmt::format("##SeedFinderAccessible{}{}", prefix, fmt::ptr(filter)).c_str(), chosenAccessibility))
        {
            for (auto i = 0; i < IM_ARRAYSIZE(accessibilityOptions); ++i)
            {
                bool isSelected = (chosenAccessibility == accessibilityOptions[i]);
                if (ImGui::Selectable(accessibilityOptions[i], isSelected))
                {
                    switch (i)
                    {
                    case 0:
                        *accessibility = AccessibilityChoice::MAYBE;
                        break;
                    case 1:
                        *accessibility = AccessibilityChoice::YES;
                        break;
                    case 2:
                        *accessibility = AccessibilityChoice::NO;
                        break;
                    }
                }
            }
            ImGui::EndCombo();
        }
        if (suffix != nullptr)
        {
            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            ImGui::Text(suffix);
        }
    }

    void FilterUI::renderPositionChoice(PositionChoice* position, Filter* filter, const char* prefix, const char* suffix)
    {
        static const char* positionOptions[] = {"wherever", "higher", "lower"};
        const char* chosenPosition = positionOptions[0];
        if (*position == PositionChoice::HIGHER)
        {
            chosenPosition = positionOptions[1];
        }
        else if (*position == PositionChoice::LOWER)
        {
            chosenPosition = positionOptions[2];
        }

        ImGui::AlignTextToFramePadding();
        ImGui::Text(prefix);
        ImGui::SameLine();

        ImGui::PushItemWidth(145);
        if (ImGui::BeginCombo(fmt::format("##SeedFinderPosition{}{}{}", prefix, suffix, fmt::ptr(filter)).c_str(), chosenPosition))
        {
            for (auto i = 0; i < IM_ARRAYSIZE(positionOptions); ++i)
            {
                bool isSelected = (chosenPosition == positionOptions[i]);
                if (ImGui::Selectable(positionOptions[i], isSelected))
                {
                    switch (i)
                    {
                    case 0:
                        *position = PositionChoice::WHEREVER;
                        break;
                    case 1:
                        *position = PositionChoice::HIGHER;
                        break;
                    case 2:
                        *position = PositionChoice::LOWER;
                        break;
                    }
                }
            }
            ImGui::EndCombo();
        }
        if (suffix != nullptr)
        {
            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            ImGui::Text(suffix);
        }
    }

} // namespace SeedFinder