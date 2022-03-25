#include "FilterFindSisters.h"
#include "AStar/SimplifiedLevel.h"
#include "SeedFinder.h"
#include <iostream>

namespace SeedFinder
{
    uint16_t FilterFindSisters::msSisterParsleyID = 0;
    uint16_t FilterFindSisters::msSisterParsnipID = 0;
    uint16_t FilterFindSisters::msSisterParmesanID = 0;
    uint16_t FilterFindSisters::msLayerDoorID = 0;

    FilterFindSisters::FilterFindSisters(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msSisterParsleyID == 0)
        {
            msSisterParsleyID = static_cast<uint16_t>(to_id("ENT_TYPE_MONS_SISTER_PARSLEY"));
            msSisterParsnipID = static_cast<uint16_t>(to_id("ENT_TYPE_MONS_SISTER_PARSNIP"));
            msSisterParmesanID = static_cast<uint16_t>(to_id("ENT_TYPE_MONS_SISTER_PARMESAN"));
            msLayerDoorID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_LAYER"));
        }
        mLevelsToSearch.hideLevel(1, 1);
        mLevelsToSearch.hideLevel(1, 2);
        mLevelsToSearch.hideLevel(1, 3);
        mLevelsToSearch.hideLevel(1, 4);
        mLevelsToSearch.hideLevel(2, 1);
        mLevelsToSearch.hideLevel(3, 1);
        mLevelsToSearch.hideLevel(4, 1);
        mLevelsToSearch.hideLevel(4, 2);
        mLevelsToSearch.hideLevel(4, 3);
        mLevelsToSearch.hideLevel(4, 4);
        mLevelsToSearch.hideLevel(5, 1);
        mLevelsToSearch.hideLevel(6, 1);
        mLevelsToSearch.hideLevel(6, 2);
        mLevelsToSearch.hideLevel(6, 3);
        mLevelsToSearch.hideLevel(6, 4);
        mLevelsToSearch.hideLevel(7, 1);
        mLevelsToSearch.hideLevel(7, 2);
        mLevelsToSearch.hideLevel(7, 3);
        mLevelsToSearch.hideLevel(7, 4);
    }

    std::string FilterFindSisters::uniqueIdentifier()
    {
        return "FilterFindSisters";
    }

    std::string FilterFindSisters::title()
    {
        return "Find sisters";
    }

    std::unique_ptr<FilterFindSisters> FilterFindSisters::instantiate(SeedFinder* seedFinder)
    {
        return (std::make_unique<FilterFindSisters>(seedFinder));
    }

    uint8_t FilterFindSisters::deepestLevel() const
    {
        return mLevelsToSearch.deepest();
    }

    bool FilterFindSisters::shouldExecute(uint8_t currentWorld, uint8_t currentLevel)
    {
        return mLevelsToSearch.shouldExecute(currentWorld, currentLevel);
    }

    bool FilterFindSisters::isValid()
    {
        setLastError("");
        auto levelsValid = mLevelsToSearch.isValid();
        if (!levelsValid)
        {
            setLastError("Please choose at least one level");
            return false;
        }

        if (mAccessibility == AccessibilityChoice::MAYBE)
        {
            setLastError("Please choose either must or must not be accessible, as the sister are always present");
            return false;
        }

        if (!mSeedFinder->areWeGoingToJungle())
        {
            setLastError("To find the sisters, we must go to the Jungle instead of Volcana (in the global configuration above)");
            return false;
        }

        return true;
    }

    bool FilterFindSisters::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        auto state = State::get();
        auto frontLayerEntities = state.layer(0)->all_entities.entities();
        auto backLayerEntities = state.layer(1)->all_entities.entities();

        uint8_t startDoorX = 0;
        uint8_t startDoorY = 0;
        uint8_t sisterX = 0;
        uint8_t sisterY = 0;
        uint16_t sisterID = (currentLevel == 2 ? msSisterParsleyID : (currentLevel == 3 ? msSisterParsnipID : msSisterParmesanID));

        for (Entity* entity : frontLayerEntities)
        {
            if (entity->type->id == Filter::msStartDoorID)
            {
                startDoorX = static_cast<uint8_t>(std::round(entity->x));
                startDoorY = static_cast<uint8_t>(std::round(entity->y));
                break;
            }
        }

        for (Entity* entity : backLayerEntities)
        {
            if (entity->type->id == sisterID)
            {
                sisterX = static_cast<uint8_t>(std::round(entity->x));
                sisterY = static_cast<uint8_t>(std::round(entity->y));
                Util::log(fmt::format("- FilterFindSisters: found sister on {}-{} at x:{} y:{}", currentWorld, currentLevel, entity->x, entity->y));
                break;
            }
        }
        if (sisterX == 0 && sisterY == 0)
        {
            return false; // shouldn't happen
        }

        // search for the sister's layer door
        uint8_t layerDoorX = 0;
        uint8_t layerDoorY = 0;
        auto doorSearchRect = Rect(sisterX - 4, sisterY + 2.5, 10, 2);
        for (Entity* entity : backLayerEntities)
        {
            auto eX = static_cast<uint8_t>(std::round(entity->x));
            auto eY = static_cast<uint8_t>(std::round(entity->y));
            if (entity->type->id == msLayerDoorID && doorSearchRect.containsPoint(eX, eY))
            {
                layerDoorX = eX;
                layerDoorY = eY;
                break;
            }
        }
        if (layerDoorX == 0 && layerDoorY == 0)
        {
            return false; // shouldn't happen
        }

        auto simpleLevel = SimplifiedLevel(mSeedFinder, state.ptr()->w, state.ptr()->h, frontLayerEntities);
        bool accessible = simpleLevel.areTilesConnected(startDoorX, startDoorY, layerDoorX, layerDoorY);
        Util::log(fmt::format("- FilterFindSisters: door accessible: {}", accessible));

        if (mAccessibility == AccessibilityChoice::YES)
        {
            return (accessible == true);
        }
        if (mAccessibility == AccessibilityChoice::NO)
        {
            return (accessible == false);
        }
        return false;
    }

    void FilterFindSisters::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindSistersHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);

            FilterUI::renderAccessibilityChoice(&mAccessibility, this, "Sister door", "be accessible without bombs");
            FilterUI::renderDeleteFilterButton(this);
            FilterUI::renderLevelSelectorCheckboxes(&mLevelsToSearch, false);
        }
    }

    void FilterFindSisters::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindSisters"));
        Util::log(fmt::format("\tAccessibility: {}", mAccessibility == AccessibilityChoice::MAYBE ? "ignored" : mAccessibility == AccessibilityChoice::YES ? "yes" : "no"));
        Util::log(fmt::format("\tLevel(s): {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }

    json FilterFindSisters::serialize() const
    {
        json j;
        j[SeedFinder::kJSONVersion] = 1;
        j[SeedFinder::kJSONFilterID] = uniqueIdentifier();
        j[SeedFinder::kJSONAccessibility] = static_cast<int>(mAccessibility);
        j[SeedFinder::kJSONLevels] = mLevelsToSearch.serialize();
        return j;
    }

    std::string FilterFindSisters::unserialize(const json& j)
    {
        if (j.contains(SeedFinder::kJSONVersion))
        {
            auto version = j.at(SeedFinder::kJSONVersion).get<uint8_t>();
            if (version == 1)
            {
                if (j.contains(SeedFinder::kJSONAccessibility))
                {
                    mAccessibility = static_cast<AccessibilityChoice>(j.at(SeedFinder::kJSONAccessibility).get<uint8_t>());
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