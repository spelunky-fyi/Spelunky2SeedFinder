#include "FilterFindItemInCrust.h"
#include "SeedFinder.h"
#include "rpc.hpp"

namespace SeedFinder
{
    const char* FilterFindItemInCrust::msComboItemOptions[] = {
        "",                    // 0
        "Bomb Bag",            // 1
        "Bomb Box",            // 2
        "Boomerang",           // 3
        "Camera",              // 4
        "Cape",                // 5
        "Climbing Gloves",     // 6
        "Compass",             // 7
        "Cooked Turkey",       // 8
        "Crossbow",            // 9
        "Freeze Ray",          // 10
        "Hoverpack",           // 11
        "Jetpack",             // 12
        "Machete",             // 13
        "Mattock",             // 14
        "Metal Shield",        // 15
        "Parachute",           // 16
        "Paste",               // 17
        "Pitcher's Mitt",      // 18
        "Powerpack",           // 19
        "Plasma Cannon",       // 20
        "Rope Pile",           // 21
        "Royal Jelly",         // 22
        "Shotgun",             // 23
        "Spectacles",          // 24
        "Spike Shoes",         // 25
        "Spring Shoes",        // 26
        "Teleporter",          // 27
        "Teleporter Backpack", // 28
        "Web Gun",             // 29
    };

    size_t FilterFindItemInCrust::msCrustItemIDs[cCrustItemsCount] = {0};
    FilterFindItemInCrust::FilterFindItemInCrust(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msCrustItemIDs[1] == 0)
        {
            msCrustItemIDs[0] = 0;
            msCrustItemIDs[1] = to_id("ENT_TYPE_ITEM_PICKUP_BOMBBAG");
            msCrustItemIDs[2] = to_id("ENT_TYPE_ITEM_PICKUP_BOMBBOX");
            msCrustItemIDs[3] = to_id("ENT_TYPE_ITEM_BOOMERANG");
            msCrustItemIDs[4] = to_id("ENT_TYPE_ITEM_CAMERA");
            msCrustItemIDs[5] = to_id("ENT_TYPE_ITEM_CAPE");
            msCrustItemIDs[6] = to_id("ENT_TYPE_ITEM_PICKUP_CLIMBINGGLOVES");
            msCrustItemIDs[7] = to_id("ENT_TYPE_ITEM_PICKUP_COMPASS");
            msCrustItemIDs[8] = to_id("ENT_TYPE_ITEM_PICKUP_COOKEDTURKEY");
            msCrustItemIDs[9] = to_id("ENT_TYPE_ITEM_CROSSBOW");
            msCrustItemIDs[10] = to_id("ENT_TYPE_ITEM_FREEZERAY");
            msCrustItemIDs[11] = to_id("ENT_TYPE_ITEM_HOVERPACK");
            msCrustItemIDs[12] = to_id("ENT_TYPE_ITEM_JETPACK");
            msCrustItemIDs[13] = to_id("ENT_TYPE_ITEM_MACHETE");
            msCrustItemIDs[14] = to_id("ENT_TYPE_ITEM_MATTOCK");
            msCrustItemIDs[15] = to_id("ENT_TYPE_ITEM_METAL_SHIELD");
            msCrustItemIDs[16] = to_id("ENT_TYPE_ITEM_PICKUP_PARACHUTE");
            msCrustItemIDs[17] = to_id("ENT_TYPE_ITEM_PICKUP_PASTE");
            msCrustItemIDs[18] = to_id("ENT_TYPE_ITEM_PICKUP_PITCHERSMITT");
            msCrustItemIDs[19] = to_id("ENT_TYPE_ITEM_POWERPACK");
            msCrustItemIDs[20] = to_id("ENT_TYPE_ITEM_PLASMACANNON");
            msCrustItemIDs[21] = to_id("ENT_TYPE_ITEM_PICKUP_ROPEPILE");
            msCrustItemIDs[22] = to_id("ENT_TYPE_ITEM_PICKUP_ROYALJELLY");
            msCrustItemIDs[23] = to_id("ENT_TYPE_ITEM_SHOTGUN");
            msCrustItemIDs[24] = to_id("ENT_TYPE_ITEM_PICKUP_SPECTACLES");
            msCrustItemIDs[25] = to_id("ENT_TYPE_ITEM_PICKUP_SPIKESHOES");
            msCrustItemIDs[26] = to_id("ENT_TYPE_ITEM_PICKUP_SPRINGSHOES");
            msCrustItemIDs[27] = to_id("ENT_TYPE_ITEM_TELEPORTER");
            msCrustItemIDs[28] = to_id("ENT_TYPE_ITEM_TELEPORTER_BACKPACK");
            msCrustItemIDs[29] = to_id("ENT_TYPE_ITEM_WEBGUN");
        }
    }

    std::string FilterFindItemInCrust::uniqueIdentifier()
    {
        return "FilterFindItemInCrust";
    }

    std::string FilterFindItemInCrust::title()
    {
        return "Find item in crust";
    }

    std::unique_ptr<FilterFindItemInCrust> FilterFindItemInCrust::instantiate(SeedFinder* seedFinder)
    {
        return (std::make_unique<FilterFindItemInCrust>(seedFinder));
    }

    uint8_t FilterFindItemInCrust::deepestLevel() const
    {
        return mLevelsToSearch.deepest();
    }

    bool FilterFindItemInCrust::shouldExecute(uint8_t currentWorld, uint8_t currentLevel)
    {
        return mLevelsToSearch.shouldExecute(currentWorld, currentLevel);
    }

    bool FilterFindItemInCrust::isValid()
    {
        setLastError("");
        if (mItemID == 0)
        {
            setLastError("Please choose an item");
        }
        auto levelsValid = mLevelsToSearch.isValid();
        if (!levelsValid)
        {
            setLastError("Please choose at least one level");
        }
        return (levelsValid && mItemID != 0);
    }

    bool FilterFindItemInCrust::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        auto searchCrustItem = [&](EntityList::EntityRange entities) {
            for (Entity* entity : entities)
            {
                if (entity->items.size > 0)
                {
                    for (auto uid : entity->items.uids())
                    {
                        auto testEntity = get_entity_ptr(uid);
                        if (testEntity != nullptr && testEntity->type->id == mItemID)
                        {
                            Util::log(fmt::format("- FilterFindItemInCrust: found {} at {}, {}", mSeedFinder->entityName(mItemID), entity->x, entity->y));
                            return true;
                        }
                    }
                }
            }
            return false;
        };

        bool satisfied = true;

        auto state = State::get();
        if (mLayer == LayerChoice::ALL || mLayer == LayerChoice::FRONT)
        {
            auto success = searchCrustItem(state.layer(0)->all_entities.entities());
            if (!success)
            {
                satisfied = false;
            }
        }
        if (mLayer == LayerChoice::ALL || mLayer == LayerChoice::BACK)
        {
            auto success = searchCrustItem(state.layer(1)->all_entities.entities());
            if (!success)
            {
                satisfied = false;
            }
        }

        return satisfied;
    }

    void FilterFindItemInCrust::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindItemInCrustHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Item: ");

            ImGui::SameLine();
            ImGui::PushItemWidth(120);
            if (ImGui::BeginCombo(fmt::format("##SeedFinderFilterFindItemInCrustHeaderItemID{}", fmt::ptr(this)).c_str(), mComboChosenItemID))
            {
                for (auto i = 0; i < cCrustItemsCount; ++i)
                {
                    bool isSelected = (mComboChosenItemID == msComboItemOptions[i]);
                    if (ImGui::Selectable(msComboItemOptions[i], isSelected))
                    {
                        mComboChosenItemID = msComboItemOptions[i];
                        mItemID = msCrustItemIDs[i];
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            FilterUI::renderLayerSelector(&mLayer, this);

            FilterUI::renderDeleteFilterButton(this);
            FilterUI::renderLevelSelectorCheckboxes(&mLevelsToSearch);
        }
    }

    void FilterFindItemInCrust::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindItemInCrust"));
        Util::log(fmt::format("\tItem ID: {} ({})", mItemID, mSeedFinder->entityName(mItemID)));
        Util::log(fmt::format("\tLayer: {}", mLayer == LayerChoice::ALL ? "All" : mLayer == LayerChoice::FRONT ? "Front" : "Back"));
        Util::log(fmt::format("\tLevel(s): {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }

    json FilterFindItemInCrust::serialize() const
    {
        json j;
        j[SeedFinder::kJSONVersion] = 1;
        j[SeedFinder::kJSONFilterID] = uniqueIdentifier();
        j[SeedFinder::kJSONItem] = mSeedFinder->entityName(mItemID);
        j[SeedFinder::kJSONLayer] = static_cast<int>(mLayer);
        j[SeedFinder::kJSONLevels] = mLevelsToSearch.serialize();
        return j;
    }

    std::string FilterFindItemInCrust::unserialize(const json& j)
    {
        if (j.contains(SeedFinder::kJSONVersion))
        {
            auto version = j.at(SeedFinder::kJSONVersion).get<uint8_t>();
            if (version == 1)
            {
                if (j.contains(SeedFinder::kJSONItem))
                {
                    mItemID = to_id(j.at(SeedFinder::kJSONItem).get<std::string>());
                    for (auto x = 0; x < cCrustItemsCount; ++x)
                    {
                        if (mItemID == msCrustItemIDs[x])
                        {
                            mComboChosenItemID = msComboItemOptions[x];
                        }
                    }
                }
                if (j.contains(SeedFinder::kJSONLayer))
                {
                    mLayer = static_cast<LayerChoice>(j.at(SeedFinder::kJSONLayer).get<uint8_t>());
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