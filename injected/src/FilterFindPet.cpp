#include "FilterFindPet.h"
#include "AStar/SimplifiedLevel.h"
#include <iostream>

namespace SeedFinder
{
    uint16_t FilterFindPet::msPetDogID = 0;
    uint16_t FilterFindPet::msPetCatID = 0;
    uint16_t FilterFindPet::msPetHamsterID = 0;
    uint16_t FilterFindPet::msLayerDoorID = 0;
    uint16_t FilterFindPet::msLayerLockedDoorID = 0;
    uint16_t FilterFindPet::msLayerLockedDoorPenID = 0;

    FilterFindPet::FilterFindPet(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msPetDogID == 0)
        {
            msPetDogID = static_cast<uint16_t>(to_id("ENT_TYPE_MONS_PET_DOG"));
            msPetCatID = static_cast<uint16_t>(to_id("ENT_TYPE_MONS_PET_CAT"));
            msPetHamsterID = static_cast<uint16_t>(to_id("ENT_TYPE_MONS_PET_HAMSTER"));
            msLayerDoorID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_LAYER"));
            msLayerLockedDoorID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_LOCKED"));
            msLayerLockedDoorPenID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_LOCKED_PEN"));
        }
        // don't think Tiamat and Hundun levels have pets
        mLevelsToSearch.disableLevel(6, 4);
        mLevelsToSearch.disableLevel(7, 4);
    }

    std::string FilterFindPet::title() { return "Find pet"; }

    uint8_t FilterFindPet::deepestLevel() const { return mLevelsToSearch.deepest(); }

    bool FilterFindPet::shouldExecute(uint8_t currentWorld, uint8_t currentLevel) { return mLevelsToSearch.shouldExecute(currentWorld, currentLevel); }

    bool FilterFindPet::isValid()
    {
        setLastError("");
        auto levelsValid = mLevelsToSearch.isValid();
        if (!levelsValid)
        {
            setLastError("Please choose at least one level");
        }
        return levelsValid;
    }

    bool FilterFindPet::execute(uint8_t currentWorld, uint8_t currentLevel)
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

        // search for the pet on both layers
        uint8_t petX = 0;
        uint8_t petY = 0;
        const auto searchPet = [&](const std::vector<Entity*> entities, LayerChoice layer) {
            for (Entity* entity : entities)
            {
                if (entity->type->id == msPetDogID || entity->type->id == msPetCatID || entity->type->id == msPetHamsterID)
                {
                    petX = static_cast<uint8_t>(std::round(entity->x));
                    petY = static_cast<uint8_t>(std::round(entity->y));
                    Util::log(
                        fmt::format("- FilterFindPet: found pet on {}-{} ({} layer) at x:{} y:{}", currentWorld, currentLevel, layer == LayerChoice::FRONT ? "front" : "back", entity->x, entity->y));
                    return true;
                }
            }
            return false;
        };

        LayerChoice petFoundOnLayer = LayerChoice::FRONT;
        bool petFound = searchPet(frontLayerEntities, petFoundOnLayer);
        if (!petFound)
        {
            petFoundOnLayer = LayerChoice::BACK;
            petFound = searchPet(backLayerEntities, petFoundOnLayer);
        }

        if (!petFound || mAccessibility == AccessibilityChoice::MAYBE)
        {
            return petFound;
        }

        // if the pet is on the front layer, we can do a simple accessibility check from pet to starting door
        if (petFoundOnLayer == LayerChoice::FRONT)
        {
            auto simpleLevel = SimplifiedLevel(mSeedFinder, state.ptr()->w, state.ptr()->h, frontLayerEntities);
            bool accessible = simpleLevel.areTilesConnected(startDoorX, startDoorY, petX, petY);
            Util::log(fmt::format("- FilterFindPet: pet accessible: {}", accessible));

            if (mAccessibility == AccessibilityChoice::YES)
            {
                return (accessible == true);
            }
            if (mAccessibility == AccessibilityChoice::NO)
            {
                return (accessible == false);
            }
        }
        else // if the pet is on the back layer, look which level door it is connected to, and see if that level door is accessible on the front layer
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

                    if (simpleLevelBack.areTilesConnected(eX, eY, petX, petY))
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

            Util::log(fmt::format("- FilterFindPet: pet accessible through layer door: {}", accessible));

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

    void FilterFindPet::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindPetHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);

            FilterUI::renderAccessibilityChoice(&mAccessibility, this, "Pet", "be accessible without bombs, or key to back layer door");
            FilterUI::renderDeleteFilterButton(this);
            FilterUI::renderLevelSelectorCheckboxes(&mLevelsToSearch);
        }
    }

    void FilterFindPet::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindPet"));
        Util::log(fmt::format("\tAccessibility: {}", mAccessibility == AccessibilityChoice::MAYBE ? "ignored" : mAccessibility == AccessibilityChoice::YES ? "yes" : "no"));
        Util::log(fmt::format("\tLevel(s): {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }

} // namespace SeedFinder