#include "FilterFindAltar.h"
#include "AStar/SimplifiedLevel.h"
#include "SeedFinder.h"
#include <iostream>

namespace SeedFinder
{
    uint16_t FilterFindAltar::msAltarID = 0;

    FilterFindAltar::FilterFindAltar(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msAltarID == 0)
        {
            msAltarID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_ALTAR"));
        }
        mLevelsToSearch.disableLevel(1, 1);
        mLevelsToSearch.disableLevel(1, 4);
        mLevelsToSearch.disableLevel(3, 1);
        mLevelsToSearch.disableLevel(6, 4);
        mLevelsToSearch.disableLevel(7, 4);
    }

    std::string FilterFindAltar::uniqueIdentifier()
    {
        return "FilterFindAltar";
    }

    std::string FilterFindAltar::title()
    {
        return "Find altar";
    }

    std::unique_ptr<FilterFindAltar> FilterFindAltar::instantiate(SeedFinder* seedFinder)
    {
        return (std::make_unique<FilterFindAltar>(seedFinder));
    }

    uint8_t FilterFindAltar::deepestLevel() const
    {
        return mLevelsToSearch.deepest();
    }

    bool FilterFindAltar::shouldExecute(uint8_t currentWorld, uint8_t currentLevel)
    {
        return mLevelsToSearch.shouldExecute(currentWorld, currentLevel);
    }

    bool FilterFindAltar::isValid()
    {
        setLastError("");
        auto levelsValid = mLevelsToSearch.isValid();
        if (!levelsValid)
        {
            setLastError("Please choose at least one level");
        }
        return levelsValid;
    }

    bool FilterFindAltar::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        auto state = State::get();
        auto entities = state.layer(0)->items();

        uint8_t startDoorX = 0;
        uint8_t startDoorY = 0;
        uint8_t altarX = 0;
        uint8_t altarY = 0;
        bool altarFound = false;

        for (Entity* entity : entities)
        {
            if (entity->type->id == Filter::msStartDoorID)
            {
                startDoorX = static_cast<uint8_t>(std::round(entity->x));
                startDoorY = static_cast<uint8_t>(std::round(entity->y));
            }
            else if (entity->type->id == msAltarID)
            {
                altarX = static_cast<uint8_t>(std::round(entity->x));
                altarY = static_cast<uint8_t>(std::round(entity->y));
                altarFound = true;
                Util::log(fmt::format("- FilterFindAltar: found altar on {}-{} at x:{} y:{}", currentWorld, currentLevel, entity->x, entity->y));
            }
            if (startDoorY != 0 && altarY != 0)
            {
                break;
            }
        }

        if (!altarFound || mAccessibility == AccessibilityChoice::MAYBE)
        {
            return altarFound;
        }

        auto simpleLevel = SimplifiedLevel(mSeedFinder, state.ptr()->w, state.ptr()->h, entities);
        bool accessible = simpleLevel.areTilesConnected(startDoorX, startDoorY, altarX, altarY + 1);
        Util::log(fmt::format("- FilterFindAltar: altar accessible: {}", accessible));

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

    void FilterFindAltar::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindAltarHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);

            FilterUI::renderAccessibilityChoice(&mAccessibility, this, "Altar", "be accessible without bombs");
            FilterUI::renderDeleteFilterButton(this);
            FilterUI::renderLevelSelectorCheckboxes(&mLevelsToSearch);
        }
    }

    void FilterFindAltar::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindAltar"));
        Util::log(fmt::format("\tAccessibility: {}", mAccessibility == AccessibilityChoice::MAYBE ? "ignored" : mAccessibility == AccessibilityChoice::YES ? "yes" : "no"));
        Util::log(fmt::format("\tLevel(s): {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }

    json FilterFindAltar::serialize() const
    {
        json j;
        j[SeedFinder::kJSONVersion] = 1;
        j[SeedFinder::kJSONFilterID] = uniqueIdentifier();
        j[SeedFinder::kJSONAccessibility] = static_cast<int>(mAccessibility);
        j[SeedFinder::kJSONLevels] = mLevelsToSearch.serialize();
        return j;
    }

    std::string FilterFindAltar::unserialize(const json& j)
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