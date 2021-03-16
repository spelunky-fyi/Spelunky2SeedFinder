#include "FilterFindVanHorsing.h"
#include "AStar/SimplifiedLevel.h"
#include "SeedFinder.h"
#include "state.hpp"

namespace SeedFinder
{
    uint16_t FilterFindVanHorsing::msLayerDoorID = 0;
    uint16_t FilterFindVanHorsing::msVanHorsingID = 0;
    uint16_t FilterFindVanHorsing::msKeyID = 0;

    FilterFindVanHorsing::FilterFindVanHorsing(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msLayerDoorID == 0)
        {
            msLayerDoorID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_LAYER"));
            msVanHorsingID = static_cast<uint16_t>(to_id("ENT_TYPE_MONS_OLD_HUNTER"));
            msKeyID = static_cast<uint16_t>(to_id("ENT_TYPE_ITEM_KEY"));
        }
    }

    std::string FilterFindVanHorsing::title() { return "Find Van Horsing"; }

    uint8_t FilterFindVanHorsing::deepestLevel() const
    {
        return 4; // 2-1
    }

    bool FilterFindVanHorsing::shouldExecute(uint8_t currentWorld, uint8_t currentLevel) { return (currentWorld == 2 && currentLevel == 1); }

    bool FilterFindVanHorsing::isValid()
    {
        setLastError("");
        if (mHorsingDoorAccessibility == AccessibilityChoice::MAYBE && mKeyAccessibility == AccessibilityChoice::MAYBE)
        {
            setLastError("Please choose either or both options, as Van Horsing is always present");
            return false;
        }

        if (!mSeedFinder->areWeGoingToVolcana())
        {
            setLastError("To find Van Horsing, we must go to Volcana instead of the Jungle (in the global configuration above)");
            return false;
        }
        return true;
    }

    bool FilterFindVanHorsing::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        auto state = State::get();
        auto frontLayerEntities = state.layer(0)->items();
        auto backLayerEntities = state.layer(1)->items();
        auto simpleLevel = SimplifiedLevel(mSeedFinder, state.ptr()->w, state.ptr()->h, frontLayerEntities);

        // locate the start door
        uint8_t startDoorX = 0;
        uint8_t startDoorY = 0;
        for (Entity* entity : frontLayerEntities)
        {
            if (entity->type->id == Filter::msStartDoorID)
            {
                startDoorX = static_cast<uint8_t>(std::round(entity->x));
                startDoorY = static_cast<uint8_t>(std::round(entity->y));
                break;
            }
        }

        bool horsingAccessibilitySatisfied = true;
        if (mHorsingDoorAccessibility != AccessibilityChoice::MAYBE)
        {
            // locate Van Horsing
            uint8_t horsingX = 0;
            uint8_t horsingY = 0;
            for (Entity* entity : backLayerEntities)
            {
                if (entity->type->id == msVanHorsingID)
                {
                    horsingX = static_cast<uint8_t>(std::round(entity->x));
                    horsingY = static_cast<uint8_t>(std::round(entity->y));
                    Util::log(fmt::format("- FilterFindVanHorsing: found Van Horsing at x:{} y:{}", horsingX, horsingY));
                    break;
                }
            }

            // locate his door
            uint8_t horsingDoorX = 0;
            uint8_t horsingDoorY = 0;
            bool horsingDoorAccessible = false;
            auto searchRect = Rect(horsingX - 2, horsingY - 4.5, 4, 2);
            for (Entity* entity : backLayerEntities)
            {
                auto x = static_cast<uint8_t>(std::round(entity->x));
                auto y = static_cast<uint8_t>(std::round(entity->y));
                if (entity->type->id == msLayerDoorID && searchRect.containsPoint(x, y))
                {
                    horsingDoorX = x;
                    horsingDoorY = y;
                    horsingDoorAccessible = simpleLevel.areTilesConnected(startDoorX, startDoorY, horsingDoorX, horsingDoorY);
                    Util::log(fmt::format("- FilterFindVanHorsing: found Van Horsing's door at x:{} y:{}; accessibility: {}", horsingDoorX, horsingDoorY, horsingDoorAccessible));
                    break;
                }
            }

            if (mHorsingDoorAccessibility == AccessibilityChoice::YES)
            {
                horsingAccessibilitySatisfied = (horsingDoorAccessible == true);
            }
            else if (mHorsingDoorAccessibility == AccessibilityChoice::NO)
            {
                horsingAccessibilitySatisfied = (horsingDoorAccessible == false);
            }
        }

        bool keyAccessibilitySatisfied = true;
        if (mKeyAccessibility != AccessibilityChoice::MAYBE)
        {
            // there can be multiple keys on the level, so check them all. they can be in different spots though, so we search the
            // entire little chunk of where the key is, to see if there's a layer door somewhere
            bool areAllKeysInaccessible = true;
            bool atLeastOneKeyAccessible = false;
            for (Entity* entity : backLayerEntities)
            {
                if (entity->type->id == msKeyID)
                {
                    auto keyX = static_cast<uint8_t>(std::round(entity->x));
                    auto keyY = static_cast<uint8_t>(std::round(entity->y));
                    Util::log(fmt::format("- FilterFindVanHorsing: found key at x:{} y:{}", keyX, keyY));
                    auto searchRect = Rect((std::floor((keyX - 3) / 10.) * 10) + 3, (std::floor((keyY - 3) / 8.) * 8) + 3, 10, 8);
                    for (Entity* entity2 : backLayerEntities)
                    {
                        if (entity2->type->id == msLayerDoorID && searchRect.containsPoint(entity2->x, entity2->y))
                        {
                            auto keyDoorX = static_cast<uint8_t>(std::round(entity2->x));
                            auto keyDoorY = static_cast<uint8_t>(std::round(entity2->y));
                            bool accessible = simpleLevel.areTilesConnected(startDoorX, startDoorY, keyDoorX, keyDoorY);
                            Util::log(fmt::format("- FilterFindVanHorsing: found key door at x:{} y:{}; accessibility: {}", keyDoorX, keyDoorY, accessible));
                            if (accessible)
                            {
                                areAllKeysInaccessible = false;
                                atLeastOneKeyAccessible = true;
                            }
                        }
                    }
                }
            }
            if (mKeyAccessibility == AccessibilityChoice::YES)
            {
                keyAccessibilitySatisfied = (atLeastOneKeyAccessible == true);
            }
            else if (mKeyAccessibility == AccessibilityChoice::NO)
            {
                keyAccessibilitySatisfied = (areAllKeysInaccessible == true);
            }
        }

        return (horsingAccessibilitySatisfied && keyAccessibilitySatisfied);
    }

    void FilterFindVanHorsing::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindVanHorsingHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);

            FilterUI::renderAccessibilityChoice(&mHorsingDoorAccessibility, this, "Van Horsing's cell door", "be accessible without bombs");

            FilterUI::renderDeleteFilterButton(this);

            ImGui::SetCursorPosX(gSectionMarginHor * 2);
            FilterUI::renderAccessibilityChoice(&mKeyAccessibility, this, "Van Horsing's key door", "be accessible without bombs");
        }
    }

    void FilterFindVanHorsing::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindVanHorsing"));
        Util::log(fmt::format("\tVan Horsing's door accessibility: {}", mHorsingDoorAccessibility == AccessibilityChoice::MAYBE ? "ignored"
                                                                        : mHorsingDoorAccessibility == AccessibilityChoice::YES ? "yes"
                                                                                                                                : "no"));
        Util::log(fmt::format("\tVan Horsing's key accessibility: {}", mKeyAccessibility == AccessibilityChoice::MAYBE ? "ignored" : mKeyAccessibility == AccessibilityChoice::YES ? "yes" : "no"));
    }
} // namespace SeedFinder