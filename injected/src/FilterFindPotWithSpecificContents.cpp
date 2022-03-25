#include "FilterFindPotWithSpecificContents.h"
#include "AStar/SimplifiedLevel.h"
#include "SeedFinder.h"

#include <iostream>

#include "entities_items.hpp"
#include "rpc.hpp"

namespace SeedFinder
{
    uint16_t FilterFindPotWithSpecificContents::msPotID = 0;
    const char* FilterFindPotWithSpecificContents::msComboItemOptions[] = {
        "Small gold nugget", "Large gold nugget", "Large emerald", "Large sapphire", "Large ruby", "Snake", "Spider", "Cobra", "Scorpion", "Alien"};
    uint16_t FilterFindPotWithSpecificContents::msComboItemOptionItemIDs[cItemCount] = {0};

    FilterFindPotWithSpecificContents::FilterFindPotWithSpecificContents(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msPotID == 0)
        {
            msComboItemOptionItemIDs[0] = to_id("ENT_TYPE_ITEM_NUGGET_SMALL");
            msComboItemOptionItemIDs[1] = to_id("ENT_TYPE_ITEM_NUGGET");
            msComboItemOptionItemIDs[2] = to_id("ENT_TYPE_ITEM_EMERALD");
            msComboItemOptionItemIDs[3] = to_id("ENT_TYPE_ITEM_SAPPHIRE");
            msComboItemOptionItemIDs[4] = to_id("ENT_TYPE_ITEM_RUBY");
            msComboItemOptionItemIDs[5] = to_id("ENT_TYPE_MONS_SNAKE");
            msComboItemOptionItemIDs[6] = to_id("ENT_TYPE_MONS_SPIDER");
            msComboItemOptionItemIDs[7] = to_id("ENT_TYPE_MONS_COBRA");
            msComboItemOptionItemIDs[8] = to_id("ENT_TYPE_MONS_SCORPION");
            msComboItemOptionItemIDs[9] = to_id("ENT_TYPE_MONS_ALIEN");

            msPotID = static_cast<uint16_t>(to_id("ENT_TYPE_ITEM_POT"));
        }
    }

    std::string FilterFindPotWithSpecificContents::uniqueIdentifier()
    {
        return "FilterFindPotWithSpecificContents";
    }

    std::string FilterFindPotWithSpecificContents::title()
    {
        return "Find pot with specific contents";
    }

    std::unique_ptr<FilterFindPotWithSpecificContents> FilterFindPotWithSpecificContents::instantiate(SeedFinder* seedFinder)
    {
        return (std::make_unique<FilterFindPotWithSpecificContents>(seedFinder));
    }

    uint8_t FilterFindPotWithSpecificContents::deepestLevel() const
    {
        return mLevelsToSearch.deepest();
    }

    bool FilterFindPotWithSpecificContents::shouldExecute(uint8_t currentWorld, uint8_t currentLevel)
    {
        return mLevelsToSearch.shouldExecute(currentWorld, currentLevel);
    }

    bool FilterFindPotWithSpecificContents::isValid()
    {
        setLastError("");
        auto levelsValid = mLevelsToSearch.isValid();
        if (!levelsValid)
        {
            setLastError("Please choose at least one level");
        }
        if (mItemID == 0)
        {
            setLastError("Please choose an item to appear in the pot");
        }
        if (mMinimumAmount == 0)
        {
            setLastError("Please choose a valid minimum amount");
        }

        return (levelsValid && mItemID != 0 && mMinimumAmount > 0);
    }

    bool FilterFindPotWithSpecificContents::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        auto state = State::get();

        uint8_t foundAmount = 0;
        const auto checkLayer = [&](const EntityList::EntityRange& items) {
            for (auto item : items)
            {
                if (item->type->id == msPotID)
                {
                    auto container = item->as<Container>();
                    if (container->inside == mItemID)
                    {
                        foundAmount++;
                        Util::log(fmt::format("- FilterFindPotWithSpecificContents: found pot containing {} on {}-{} at x:{} y:{}", mItemID, currentWorld, currentLevel, item->x, item->y));
                        if (foundAmount >= mMinimumAmount)
                        {
                            Util::log(fmt::format("- FilterFindPotWithSpecificContents: minimum amount of pots containing {} on {}-{} satisfied", mItemID, currentWorld, currentLevel));
                            return;
                        }
                    }
                }
            }
        };

        if (mLayer == LayerChoice::FRONT || mLayer == LayerChoice::ALL)
        {
            checkLayer(state.layer(0)->all_entities.entities());
        }
        if (mLayer == LayerChoice::BACK || mLayer == LayerChoice::ALL)
        {
            checkLayer(state.layer(1)->all_entities.entities());
        }
        return foundAmount >= mMinimumAmount;
    }

    void FilterFindPotWithSpecificContents::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindPotWithSpecificContentsHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);

            ImGui::AlignTextToFramePadding();
            ImGui::Text("Item in pot :");
            ImGui::SameLine();
            ImGui::PushItemWidth(120);
            if (ImGui::BeginCombo(fmt::format("##SeedFinderFilterFindPotWithSpecificContentsItemID{}", fmt::ptr(this)).c_str(), mComboChosenItemID))
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
            ImGui::Text("Min. amount of pots :");
            ImGui::SameLine();
            ImGui::PushItemWidth(30);
            ImGui::InputScalar(fmt::format("##SeedFinderFilterFindPotWithSpecificContentsMinimumAmount{}", fmt::ptr(this)).c_str(), ImGuiDataType_U8, &mMinimumAmount);

            ImGui::SameLine();
            FilterUI::renderLayerSelector(&mLayer, this);
            FilterUI::renderDeleteFilterButton(this);
            FilterUI::renderLevelSelectorCheckboxes(&mLevelsToSearch);
        }
    }

    void FilterFindPotWithSpecificContents::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindPotWithSpecificContents"));
        Util::log(fmt::format("\tItem ID: {} ({})", mItemID, mSeedFinder->entityName(mItemID)));
        Util::log(fmt::format("\tMinimum amount: {}", mMinimumAmount));
        Util::log(fmt::format("\tLayer: {}", mLayer == LayerChoice::ALL ? "All" : mLayer == LayerChoice::FRONT ? "Front" : "Back"));
        Util::log(fmt::format("\tLevel(s): {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }

    json FilterFindPotWithSpecificContents::serialize() const
    {
        json j;
        j[SeedFinder::kJSONVersion] = 1;
        j[SeedFinder::kJSONFilterID] = uniqueIdentifier();
        j[SeedFinder::kJSONItem] = mSeedFinder->entityName(mItemID);
        j[SeedFinder::kJSONLayer] = static_cast<int>(mLayer);
        j[SeedFinder::kJSONMinimum] = mMinimumAmount;
        j[SeedFinder::kJSONLevels] = mLevelsToSearch.serialize();
        return j;
    }

    std::string FilterFindPotWithSpecificContents::unserialize(const json& j)
    {
        if (j.contains(SeedFinder::kJSONVersion))
        {
            auto version = j.at(SeedFinder::kJSONVersion).get<uint8_t>();
            if (version == 1)
            {
                if (j.contains(SeedFinder::kJSONItem))
                {
                    mItemID = to_id(j.at(SeedFinder::kJSONItem).get<std::string>());
                    for (auto x = 0; x < cItemCount; ++x)
                    {
                        if (mItemID == msComboItemOptionItemIDs[x])
                        {
                            mComboChosenItemID = msComboItemOptions[x];
                        }
                    }
                }
                if (j.contains(SeedFinder::kJSONLayer))
                {
                    mLayer = static_cast<LayerChoice>(j.at(SeedFinder::kJSONLayer).get<uint8_t>());
                }
                if (j.contains(SeedFinder::kJSONMinimum))
                {
                    mMinimumAmount = j.at(SeedFinder::kJSONMinimum).get<uint8_t>();
                }
                if (j.contains(SeedFinder::kJSONLevels))
                {
                    mLevelsToSearch.unserialize(j.at(SeedFinder::kJSONLevels));
                }
            }
            else
            {
                return fmt::format("Version mismatch for {}, can't read this version", uniqueIdentifier());
            }
        }
        else
        {
            return fmt::format("No version number specified for {}", uniqueIdentifier());
        }
        return "";
    }

} // namespace SeedFinder