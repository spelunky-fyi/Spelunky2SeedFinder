#include "FilterFindCrateWithSpecificContents.h"
#include "FilterUI.h"
#include "SeedFinder.h"

namespace SeedFinder
{
    const char* FilterFindCrateWithSpecificContents::msComboItemOptions[] = {"Bomb Bag",   "Bomb Box",  "Boomerang", "Camera",     "Cape",        "Climbing Gloves", "Compass",        "Crossbow",
                                                                             "Freeze Ray", "Jetpack",   "Machete",   "Mattock",    "Parachute",   "Paste",           "Pitcher's Mitt", "Plasma Cannon",
                                                                             "Powerpack",  "Rope Pile", "Shotgun",   "Spectacles", "Spike Shoes", "Spring Shoes",    "Teleporter",     "Web Gun"};
    uint16_t FilterFindCrateWithSpecificContents::msComboItemOptionItemIDs[cItemCount] = {0};
    uint16_t FilterFindCrateWithSpecificContents::msCrateID = 0;

    FilterFindCrateWithSpecificContents::FilterFindCrateWithSpecificContents(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msComboItemOptionItemIDs[0] == 0)
        {
            msComboItemOptionItemIDs[0] = to_id("ENT_TYPE_ITEM_PICKUP_BOMBBAG");
            msComboItemOptionItemIDs[1] = to_id("ENT_TYPE_ITEM_PICKUP_BOMBBOX");
            msComboItemOptionItemIDs[2] = to_id("ENT_TYPE_ITEM_BOOMERANG");
            msComboItemOptionItemIDs[3] = to_id("ENT_TYPE_ITEM_CAMERA");
            msComboItemOptionItemIDs[4] = to_id("ENT_TYPE_ITEM_CAPE");
            msComboItemOptionItemIDs[5] = to_id("ENT_TYPE_ITEM_PICKUP_CLIMBINGGLOVES");
            msComboItemOptionItemIDs[6] = to_id("ENT_TYPE_ITEM_PICKUP_COMPASS");
            msComboItemOptionItemIDs[7] = to_id("ENT_TYPE_ITEM_CROSSBOW");
            msComboItemOptionItemIDs[8] = to_id("ENT_TYPE_ITEM_FREEZERAY");
            msComboItemOptionItemIDs[9] = to_id("ENT_TYPE_ITEM_JETPACK");
            msComboItemOptionItemIDs[10] = to_id("ENT_TYPE_ITEM_MACHETE");
            msComboItemOptionItemIDs[11] = to_id("ENT_TYPE_ITEM_MATTOCK");
            msComboItemOptionItemIDs[12] = to_id("ENT_TYPE_ITEM_PICKUP_PARACHUTE");
            msComboItemOptionItemIDs[13] = to_id("ENT_TYPE_ITEM_PICKUP_PASTE");
            msComboItemOptionItemIDs[14] = to_id("ENT_TYPE_ITEM_PICKUP_PITCHERSMITT");
            msComboItemOptionItemIDs[15] = to_id("ENT_TYPE_ITEM_PLASMACANNON");
            msComboItemOptionItemIDs[16] = to_id("ENT_TYPE_ITEM_POWERPACK");
            msComboItemOptionItemIDs[17] = to_id("ENT_TYPE_ITEM_PICKUP_ROPEPILE");
            msComboItemOptionItemIDs[18] = to_id("ENT_TYPE_ITEM_SHOTGUN");
            msComboItemOptionItemIDs[19] = to_id("ENT_TYPE_ITEM_PICKUP_SPECTACLES");
            msComboItemOptionItemIDs[20] = to_id("ENT_TYPE_ITEM_PICKUP_SPIKESHOES");
            msComboItemOptionItemIDs[21] = to_id("ENT_TYPE_ITEM_PICKUP_SPRINGSHOES");
            msComboItemOptionItemIDs[22] = to_id("ENT_TYPE_ITEM_TELEPORTER");
            msComboItemOptionItemIDs[23] = to_id("ENT_TYPE_ITEM_WEBGUN");
            msCrateID = to_id("ENT_TYPE_ITEM_CRATE");
        }
    }

    std::string FilterFindCrateWithSpecificContents::title() { return "Find crate with specific contents"; }

    uint8_t FilterFindCrateWithSpecificContents::deepestLevel() const { return mLevelsToSearch.deepest(); }

    bool FilterFindCrateWithSpecificContents::shouldExecute(uint8_t currentWorld, uint8_t currentLevel) { return mLevelsToSearch.shouldExecute(currentWorld, currentLevel); }

    bool FilterFindCrateWithSpecificContents::isValid()
    {
        setLastError("");
        auto levelsValid = mLevelsToSearch.isValid();
        if (!levelsValid)
        {
            setLastError("Please choose at least one level");
        }
        if (mItemID == 0)
        {
            setLastError("Please choose an item to appear in the crate");
        }
        if (mMinimumAmount == 0)
        {
            setLastError("Please choose a valid minimum amount");
        }

        return (levelsValid && mItemID != 0 && mMinimumAmount > 0);
    }

    bool FilterFindCrateWithSpecificContents::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        auto state = State::get();

        uint8_t foundAmount = 0;
        const auto checkLayer = [&](const std::vector<Entity*>& items) {
            for (auto item : items)
            {
                if (item->type->id == msCrateID)
                {
                    auto container = item->as<Container>();
                    if (container->inside == mItemID)
                    {
                        foundAmount++;
                        Util::log(fmt::format("- FilterFindCrateWithSpecificContents: found crate containing {} on {}-{} at x:{} y:{}", mItemID, currentWorld, currentLevel, item->x, item->y));
                        if (foundAmount >= mMinimumAmount)
                        {
                            Util::log(fmt::format("- FilterFindCrateWithSpecificContents: minimum amount of crates containing {} on {}-{} satisfied", mItemID, currentWorld, currentLevel));
                            return;
                        }
                    }
                }
            }
        };

        if (mLayer == LayerChoice::FRONT || mLayer == LayerChoice::ALL)
        {
            checkLayer(state.layer(0)->items());
        }
        if (mLayer == LayerChoice::BACK || mLayer == LayerChoice::ALL)
        {
            checkLayer(state.layer(1)->items());
        }
        return foundAmount >= mMinimumAmount;
    }

    void FilterFindCrateWithSpecificContents::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindCrateWithSpecificContentsHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);

            ImGui::AlignTextToFramePadding();
            ImGui::Text("Item in crate :");
            ImGui::SameLine();
            ImGui::PushItemWidth(120);
            if (ImGui::BeginCombo(fmt::format("##SeedFinderFilterFindCrateWithSpecificContentsItemID{}", fmt::ptr(this)).c_str(), mComboChosenItemID))
            {
                for (auto i = 0; i < IM_ARRAYSIZE(msComboItemOptions); ++i)
                {
                    bool isSelected = (mComboChosenItemID == msComboItemOptions[i]);
                    if (ImGui::Selectable(msComboItemOptions[i], isSelected))
                    {
                        mComboChosenItemID = msComboItemOptions[i];
                        mItemID = msComboItemOptionItemIDs[i];
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Min. amount of crates :");
            ImGui::SameLine();
            ImGui::PushItemWidth(30);
            ImGui::InputScalar(fmt::format("##SeedFinderFilterFindCrateWithSpecificContentsMinimumAmount{}", fmt::ptr(this)).c_str(), ImGuiDataType_U8, &mMinimumAmount);

            ImGui::SameLine();
            FilterUI::renderLayerSelector(&mLayer, this);
            FilterUI::renderDeleteFilterButton(this);
            FilterUI::renderLevelSelectorCheckboxes(&mLevelsToSearch);
        }
    }

    void FilterFindCrateWithSpecificContents::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindCrateWithSpecificContents"));
        Util::log(fmt::format("\tItem ID: {} ({})", mItemID, mSeedFinder->entityName(mItemID)));
        Util::log(fmt::format("\tMinimum amount: {}", mMinimumAmount));
        Util::log(fmt::format("\tLayer: {}", mLayer == LayerChoice::ALL ? "All" : mLayer == LayerChoice::FRONT ? "Front" : "Back"));
        Util::log(fmt::format("\tLevel(s): {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }
} // namespace SeedFinder