#include "FilterFindGhostJar.h"
#include "AStar/SimplifiedLevel.h"
#include <iostream>

namespace SeedFinder
{
    uint16_t FilterFindGhostJar::msGhostJarID = 0;
    uint16_t FilterFindGhostJar::msLayerDoorID = 0;
    uint16_t FilterFindGhostJar::msLayerLockedDoorID = 0;
    uint16_t FilterFindGhostJar::msLayerLockedDoorPenID = 0;

    FilterFindGhostJar::FilterFindGhostJar(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msGhostJarID == 0)
        {
            msGhostJarID = static_cast<uint16_t>(to_id("ENT_TYPE_ITEM_CURSEDPOT"));
            msLayerDoorID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_LAYER"));
            msLayerLockedDoorID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_LOCKED"));
            msLayerLockedDoorPenID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_LOCKED_PEN"));
        }
        // don't think Tiamat and Hundun levels have ghost jars
        mLevelsToSearch.disableLevel(6, 4);
        mLevelsToSearch.disableLevel(7, 4);
    }

    std::string FilterFindGhostJar::title() { return "Find ghost jar"; }

    uint8_t FilterFindGhostJar::deepestLevel() const { return mLevelsToSearch.deepest(); }

    bool FilterFindGhostJar::shouldExecute(uint8_t currentWorld, uint8_t currentLevel) { return mLevelsToSearch.shouldExecute(currentWorld, currentLevel); }

    bool FilterFindGhostJar::isValid()
    {
        setLastError("");
        auto levelsValid = mLevelsToSearch.isValid();
        if (!levelsValid)
        {
            setLastError("Please choose at least one level");
        }
        return levelsValid;
    }

    bool FilterFindGhostJar::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        auto state = State::get();
        auto frontLayerEntities = state.layer(0)->items();
        auto backLayerEntities = state.layer(1)->items();

        // search for start door, in case we want to check accessibility
        uint8_t startDoorX = 0;
        uint8_t startDoorY = 0;
        if (mAccessibility != AccessibilityChoice::MAYBE)
        {
            for (Entity* entity : frontLayerEntities)
            {
                if (entity->type->id == Filter::msStartDoorID)
                {
                    startDoorX = static_cast<uint8_t>(std::round(entity->x));
                    startDoorY = static_cast<uint8_t>(std::round(entity->y));
                    break;
                }
            }
        }

        // search for the ghost jar on both layers
        uint8_t ghostJarX = 0;
        uint8_t ghostJarY = 0;
        const auto searchGhostJar = [&](const std::vector<Entity*> entities, LayerChoice layer) {
            for (Entity* entity : entities)
            {
                if (entity->type->id == msGhostJarID)
                {
                    ghostJarX = static_cast<uint8_t>(std::round(entity->x));
                    ghostJarY = static_cast<uint8_t>(std::round(entity->y));
                    Util::log(fmt::format("- FilterFindGhostJar: found ghost jar on {}-{} ({} layer) at x:{} y:{}", currentWorld, currentLevel, layer == LayerChoice::FRONT ? "front" : "back",
                                          entity->x, entity->y));
                    return true;
                }
            }
            return false;
        };

        LayerChoice ghostJarFoundOnLayer = LayerChoice::FRONT;
        bool ghostJarFound = searchGhostJar(frontLayerEntities, ghostJarFoundOnLayer);
        if (!ghostJarFound)
        {
            ghostJarFoundOnLayer = LayerChoice::BACK;
            ghostJarFound = searchGhostJar(backLayerEntities, ghostJarFoundOnLayer);
        }

        if (!ghostJarFound || mAccessibility == AccessibilityChoice::MAYBE)
        {
            return ghostJarFound;
        }

        // if the ghost jar is on the front layer, we can do a simple accessibility check from ghost jar to starting door
        if (ghostJarFoundOnLayer == LayerChoice::FRONT)
        {
            auto simpleLevel = SimplifiedLevel(mSeedFinder, state.ptr()->w, state.ptr()->h, frontLayerEntities);
            bool accessible = simpleLevel.areTilesConnected(startDoorX, startDoorY, ghostJarX, ghostJarY);
            Util::log(fmt::format("- FilterFindGhostJar: ghost jar accessible: {}", accessible));

            if (mAccessibility == AccessibilityChoice::YES)
            {
                return (accessible == true);
            }
            if (mAccessibility == AccessibilityChoice::NO)
            {
                return (accessible == false);
            }
        }
        else // if the ghost jar is on the back layer, look which level door it is connected to, and see if that level door is accessible on the front
             // layer
        {
            auto simpleLevelBack = SimplifiedLevel(mSeedFinder, state.ptr()->w, state.ptr()->h, backLayerEntities);

            std::vector<std::pair<uint8_t, uint8_t>> lockedDoors;
            for (Entity* entity : frontLayerEntities)
            {
                if (entity->type->id == msLayerLockedDoorID || entity->type->id == msLayerLockedDoorPenID)
                {
                    lockedDoors.emplace_back(std::make_pair(static_cast<uint8_t>(std::round(entity->x)), static_cast<uint8_t>(std::round(entity->y))));
                }
            }

            uint8_t layerDoorX = 0;
            uint8_t layerDoorY = 0;
            for (Entity* entity : backLayerEntities)
            {
                if (entity->type->id == msLayerDoorID)
                {
                    auto eX = static_cast<uint8_t>(std::round(entity->x));
                    auto eY = static_cast<uint8_t>(std::round(entity->y));
                    bool doorIsLocked = false;
                    for (const auto& lockedDoor : lockedDoors)
                    {
                        if (lockedDoor.first == eX && lockedDoor.second == eY)
                        {
                            doorIsLocked = true;
                            break;
                        }
                    }
                    if (doorIsLocked)
                    {
                        continue;
                    }

                    if (simpleLevelBack.areTilesConnected(eX, eY, ghostJarX, ghostJarY))
                    {
                        layerDoorX = eX;
                        layerDoorY = eY;
                        break;
                    }
                }
            }

            bool accessible = false;
            if (layerDoorX != 0 && layerDoorY != 0)
            {
                auto simpleLevelFront = SimplifiedLevel(mSeedFinder, state.ptr()->w, state.ptr()->h, frontLayerEntities);
                accessible = simpleLevelFront.areTilesConnected(startDoorX, startDoorY, layerDoorX, layerDoorY);
            }

            Util::log(fmt::format("- FilterFindGhostJar: ghost jar accessible through layer door: {}", accessible));

            if (mAccessibility == AccessibilityChoice::YES)
            {
                return (accessible == true);
            }
            if (mAccessibility == AccessibilityChoice::NO)
            {
                return (accessible == false);
            }
        }

        return false;
    }

    void FilterFindGhostJar::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindGhostJarHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);

            FilterUI::renderAccessibilityChoice(&mAccessibility, this, "Ghost jar", "be accessible without bombs, or key to back layer door");
            FilterUI::renderDeleteFilterButton(this);
            FilterUI::renderLevelSelectorCheckboxes(&mLevelsToSearch);
        }
    }

    void FilterFindGhostJar::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindGhostJar"));
        Util::log(fmt::format("\tAccessibility: {}", mAccessibility == AccessibilityChoice::MAYBE ? "ignored" : mAccessibility == AccessibilityChoice::YES ? "yes" : "no"));
        Util::log(fmt::format("\tLevel(s): {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }

} // namespace SeedFinder