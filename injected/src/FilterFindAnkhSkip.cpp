#include "FilterFindAnkhSkip.h"
#include "SeedFinder.h"
#include "state.hpp"

namespace SeedFinder
{
    uint16_t FilterFindAnkhSkip::msLayerDoorID = 0;
    uint16_t FilterFindAnkhSkip::msPagodaID = 0;
    const char* FilterFindAnkhSkip::kJSONCheckLayerDoor = "check_layer_door";
    const char* FilterFindAnkhSkip::kJSONCheckSidePagoda = "check_side_pagoda";

    FilterFindAnkhSkip::FilterFindAnkhSkip(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msLayerDoorID == 0)
        {
            msLayerDoorID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_LAYER"));
            msPagodaID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOORSTYLED_PAGODA"));
        }
    }

    std::string FilterFindAnkhSkip::uniqueIdentifier()
    {
        return "FilterFindAnkhSkip";
    }

    std::string FilterFindAnkhSkip::title()
    {
        return "Find Ankh skip";
    }

    std::unique_ptr<FilterFindAnkhSkip> FilterFindAnkhSkip::instantiate(SeedFinder* seedFinder)
    {
        return (std::make_unique<FilterFindAnkhSkip>(seedFinder));
    }

    uint8_t FilterFindAnkhSkip::deepestLevel() const
    {
        return 11; // 4-3
    }

    bool FilterFindAnkhSkip::shouldExecute(uint8_t currentWorld, uint8_t currentLevel)
    {
        return (currentWorld == 4 && currentLevel == 3);
    }

    bool FilterFindAnkhSkip::isValid()
    {
        setLastError("");
        auto oneOptionChecked = (mCheckLayerDoor || mCheckSidePagoda);
        if (!oneOptionChecked)
        {
            setLastError("Please choose at least one Ankh skip method");
            return false;
        }

        auto goingToTidePool = mSeedFinder->areWeGoingToTidePool();
        if (!goingToTidePool)
        {
            setLastError("To find an Ankh skip, we must go to Tide Pool (see configuration above)");
            return false;
        }
        return (oneOptionChecked && goingToTidePool);
    }

    bool FilterFindAnkhSkip::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        auto state = State::get();
        auto frontLayerEntities = state.layer(0)->items();

        auto layerDoorSatisfied = true;
        if (mCheckLayerDoor) // can't seem to find a seed that satisfies this, maybe layer doors below the lava have been patched out
        {
            uint8_t doorCount = 0;
            auto searchRect = Rect(3, 75, state.ptr()->w * 10, 7);
            for (Entity* entity : frontLayerEntities)
            {
                auto x = static_cast<uint8_t>(std::round(entity->x));
                auto y = static_cast<uint8_t>(std::round(entity->y));
                if (searchRect.containsPoint(x, y) && entity->type->id == msLayerDoorID)
                {
                    Util::log(fmt::format("- FilterFindAnkhSkip: found back layer door at x:{} y:{}", entity->x, entity->y));
                    doorCount++;
                }
            }
            layerDoorSatisfied = (doorCount > 1);
        }

        auto sidePagodaSatisfied = true;
        if (mCheckSidePagoda)
        {
            sidePagodaSatisfied = false;
            auto searchRectLeft = Rect(4, 79, 3, 3);
            auto searchRectRight = Rect(38, 79, 3, 3);
            for (Entity* entity : frontLayerEntities)
            {
                auto x = static_cast<uint8_t>(std::round(entity->x));
                auto y = static_cast<uint8_t>(std::round(entity->y));
                if (entity->type->id == msPagodaID && (searchRectLeft.containsPoint(x, y) || searchRectRight.containsPoint(x, y)))
                {
                    Util::log(fmt::format("- FilterFindAnkhSkip: found side pagoda at x:{} y:{}", entity->x, entity->y));
                    sidePagodaSatisfied = true;
                    break;
                }
            }
        }

        return (layerDoorSatisfied && sidePagodaSatisfied);
    }

    void FilterFindAnkhSkip::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindAnkhSkipHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);

            ImGui::Checkbox(fmt::format("Check for back layer doors under the lava##SeedFinderFilterFindAnkhBackLayerDoorsCheckbox{}", fmt::ptr(this)).c_str(), &mCheckLayerDoor);
            FilterUI::renderDeleteFilterButton(this);

            ImGui::SetCursorPosX(gSectionMarginHor * 2);
            ImGui::Checkbox(fmt::format("Check for a pagoda at the edge##SeedFinderFilterFindAnkhSidePagodaCheckbox{}", fmt::ptr(this)).c_str(), &mCheckSidePagoda);
        }
    }

    void FilterFindAnkhSkip::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindAnkhSkip"));
        Util::log(fmt::format("\tCheck back layer door under lava: {}", mCheckLayerDoor));
        Util::log(fmt::format("\tCheck pagoda at the side: {}", mCheckSidePagoda));
    }

    json FilterFindAnkhSkip::serialize() const
    {
        json j;
        j[SeedFinder::kJSONVersion] = 1;
        j[SeedFinder::kJSONFilterID] = uniqueIdentifier();
        j[kJSONCheckLayerDoor] = mCheckLayerDoor;
        j[kJSONCheckSidePagoda] = mCheckSidePagoda;
        return j;
    }

    std::string FilterFindAnkhSkip::unserialize(const json& j)
    {
        if (j.contains(SeedFinder::kJSONVersion))
        {
            auto version = j.at(SeedFinder::kJSONVersion).get<uint8_t>();
            if (version == 1)
            {
                if (j.contains(kJSONCheckLayerDoor))
                {
                    mCheckLayerDoor = j.at(kJSONCheckLayerDoor).get<bool>();
                }
                if (j.contains(kJSONCheckSidePagoda))
                {
                    mCheckSidePagoda = j.at(kJSONCheckSidePagoda).get<bool>();
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