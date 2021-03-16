#include "FilterFindVault.h"

namespace SeedFinder
{
    uint16_t FilterFindVault::msVaultChestID = 0;

    FilterFindVault::FilterFindVault(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msVaultChestID == 0)
        {
            msVaultChestID = static_cast<uint16_t>(to_id("ENT_TYPE_ITEM_VAULTCHEST"));
        }
        // no vaults on Quillback, Olmec, Tiamat, Hundun
        mLevelsToSearch.disableLevel(1, 4);
        mLevelsToSearch.disableLevel(3, 1);
        mLevelsToSearch.disableLevel(6, 4);
        mLevelsToSearch.disableLevel(7, 4);
    }

    std::string FilterFindVault::title() { return "Find vault"; }

    uint8_t FilterFindVault::deepestLevel() const { return mLevelsToSearch.deepest(); }

    bool FilterFindVault::shouldExecute(uint8_t currentWorld, uint8_t currentLevel) { return mLevelsToSearch.shouldExecute(currentWorld, currentLevel); }

    bool FilterFindVault::isValid()
    {
        setLastError("");
        auto levelsValid = mLevelsToSearch.isValid();
        if (!levelsValid)
        {
            setLastError("Please choose at least one level");
        }
        return levelsValid;
    }

    bool FilterFindVault::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        auto state = State::get();
        auto entities = state.layer(1)->items();

        for (Entity* entity : entities)
        {
            if (entity->type->id == msVaultChestID)
            {
                Util::log(fmt::format("- FilterFindVault: found vault chest on {}-{} at x:{} y:{}", currentWorld, currentLevel, entity->x, entity->y));
                return true;
            }
        }
        return false;
    }

    void FilterFindVault::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindVaultHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::NewLine();
            FilterUI::renderDeleteFilterButton(this);
            FilterUI::renderLevelSelectorCheckboxes(&mLevelsToSearch);
        }
    }

    void FilterFindVault::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindVault"));
        Util::log(fmt::format("\tLevel(s): {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }
} // namespace SeedFinder