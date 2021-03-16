#include "FilterFindUdjatEye.h"
#include "AStar/SimplifiedLevel.h"
#include "SeedFinder.h"

namespace SeedFinder
{
    uint16_t FilterFindUdjatEye::msUdjatKeyID = 0;
    uint16_t FilterFindUdjatEye::msUdjatChestID = 0;
    uint16_t FilterFindUdjatEye::msLayerDoorID = 0;

    FilterFindUdjatEye::FilterFindUdjatEye(SeedFinder* seedFinder) : Filter(seedFinder), mUdjatKeyFound(false)
    {
        if (msUdjatKeyID == 0)
        {
            msUdjatKeyID = static_cast<uint16_t>(to_id("ENT_TYPE_ITEM_LOCKEDCHEST_KEY"));
            msUdjatChestID = static_cast<uint16_t>(to_id("ENT_TYPE_ITEM_LOCKEDCHEST"));
            msLayerDoorID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_LAYER"));
        }
        mLevelsToSearch.hideLevel(1, 1);
        mLevelsToSearch.hideLevel(1, 4);
        mLevelsToSearch.hideLevel(2, 1);
        mLevelsToSearch.hideLevel(2, 2);
        mLevelsToSearch.hideLevel(2, 3);
        mLevelsToSearch.hideLevel(2, 4);
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
        mLevelsToSearch.Any = true;
    }

    std::string FilterFindUdjatEye::title() { return "Find Udjat Eye"; }

    void FilterFindUdjatEye::resetForNewSeed(uint32_t newSeed)
    {
        mUdjatKeyFound = false;
        mUdjatKeyFoundOnLevel = 0;
    }

    uint8_t FilterFindUdjatEye::deepestLevel() const
    {
        if (mLevelsToSearch.Any)
        {
            return 2; // 1-3
        }
        else
        {
            return mLevelsToSearch.deepest();
        }
    }

    bool FilterFindUdjatEye::shouldExecute(uint8_t currentWorld, uint8_t currentLevel)
    {
        if (currentWorld == 1 && (currentLevel == 2 || currentLevel == 3))
        {
            mCurrentWorld = currentWorld;
            mCurrentLevel = currentLevel;

            // see if there is an udjat key on this level
            if (!mUdjatKeyFound)
            {
                locateUdjatKeyAndStartDoor();
            }

            if (mLevelsToSearch.Any)
            {
                // if 'Any', only execute when the udjat eye is actually here, otherwise the two other levels
                // would execute and return false, making it so the seed won't get picked
                return mUdjatKeyFoundOnLevel == currentLevel;
            }
            else
            {
                // if a specific level was forced, it must be executed, so that the seed won't get picked
                // if the udjat eye isn't there
                return mLevelsToSearch.shouldExecute(currentWorld, currentLevel);
            }
        }
        else
        {
            return false;
        }
    }

    bool FilterFindUdjatEye::isValid() { return true; }

    bool FilterFindUdjatEye::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        mCurrentWorld = currentWorld;
        mCurrentLevel = currentLevel;

        if (!mUdjatKeyFound || mUdjatKeyFoundOnLevel != currentLevel)
        {
            return false;
        }

        bool levelSatisfied = mLevelsToSearch.Any || ((mLevelsToSearch.OneTwo && mUdjatKeyFoundOnLevel == 2) || (mLevelsToSearch.OneThree && mUdjatKeyFoundOnLevel == 3));

        if (!levelSatisfied)
        {
            return false;
        }

        if (mDoorAccessibility != AccessibilityChoice::MAYBE || mKeyPosition != PositionChoice::WHEREVER)
        {
            locateUdjatDoor();
        }

        auto state = State::get();
        auto simpleLevel = SimplifiedLevel(mSeedFinder, state.ptr()->w, state.ptr()->h, mFrontLayerEntities);

        bool keyAccessibilitySatisfied = false;

        if (mKeyAccessibility == AccessibilityChoice::MAYBE)
        {
            keyAccessibilitySatisfied = true;
        }
        else
        {
            bool keyAccessible = simpleLevel.areTilesConnected(mStartDoorX, mStartDoorY, mUdjatKeyX, mUdjatKeyY);
            Util::log(fmt::format("- FilterFindUdjatEye: key accessible: {}", keyAccessible));

            if (mKeyAccessibility == AccessibilityChoice::YES)
            {
                keyAccessibilitySatisfied = (keyAccessible == true);
            }
            if (mKeyAccessibility == AccessibilityChoice::NO)
            {
                keyAccessibilitySatisfied = (keyAccessible == false);
            }
        }
        if (!keyAccessibilitySatisfied)
        {
            return false;
        }

        bool doorAccessibilitySatisfied = false;
        if (mDoorAccessibility == AccessibilityChoice::MAYBE)
        {
            doorAccessibilitySatisfied = true;
        }
        else
        {
            bool doorAccessible = simpleLevel.areTilesConnected(mStartDoorX, mStartDoorY, mUdjatDoorX, mUdjatDoorY);
            Util::log(fmt::format("- FilterFindUdjatEye: door accessible: {}", doorAccessible));

            if (mDoorAccessibility == AccessibilityChoice::YES)
            {
                doorAccessibilitySatisfied = (doorAccessible == true);
            }
            if (mDoorAccessibility == AccessibilityChoice::NO)
            {
                doorAccessibilitySatisfied = (doorAccessible == false);
            }
        }
        if (!doorAccessibilitySatisfied)
        {
            return false;
        }

        bool keyPositionSatisfied = false;
        if (mKeyPosition == PositionChoice::WHEREVER)
        {
            keyPositionSatisfied = true;
        }
        else
        {
            Util::log(fmt::format("- FilterFindUdjatEye: key is {} than door", (mUdjatKeyY >= mUdjatDoorY ? "higher" : "lower")));
            if (mKeyPosition == PositionChoice::HIGHER)
            {
                keyPositionSatisfied = (mUdjatKeyY >= mUdjatDoorY);
            }
            else if (mKeyPosition == PositionChoice::LOWER)
            {
                keyPositionSatisfied = (mUdjatKeyY < mUdjatDoorY);
            }
        }
        if (!keyPositionSatisfied)
        {
            return false;
        }

        return (levelSatisfied && keyAccessibilitySatisfied && doorAccessibilitySatisfied && keyPositionSatisfied);
    }

    void FilterFindUdjatEye::locateUdjatKeyAndStartDoor()
    {
        mStartDoorX = 0;
        mStartDoorY = 0;
        mUdjatKeyX = 0;
        mUdjatKeyY = 0;

        auto state = State::get();
        mFrontLayerEntities = state.layer(0)->items();
        for (Entity* entity : mFrontLayerEntities)
        {
            if (entity->type->id == Filter::msStartDoorID)
            {
                mStartDoorX = static_cast<uint8_t>(std::round(entity->x));
                mStartDoorY = static_cast<uint8_t>(std::round(entity->y));
            }
            else if (entity->type->id == msUdjatKeyID)
            {
                mUdjatKeyX = static_cast<uint8_t>(std::round(entity->x));
                mUdjatKeyY = static_cast<uint8_t>(std::round(entity->y));
                mUdjatKeyFound = true;
                mUdjatKeyFoundOnLevel = mCurrentLevel;
                Util::log(fmt::format("- FilterFindUdjatEye: found key on {}-{} at x:{} y:{}", mCurrentWorld, mCurrentLevel, entity->x, entity->y));
            }
            if (mStartDoorY != 0 && mUdjatKeyY != 0)
            {
                break;
            }
        }
    }

    void FilterFindUdjatEye::locateUdjatDoor()
    {
        mUdjatDoorX = 0;
        mUdjatDoorY = 0;

        auto state = State::get();
        mBackLayerEntities = state.layer(1)->items();

        auto doorSearchRect = Rect();
        for (Entity* entity : mBackLayerEntities)
        {
            if (entity->type->id == msUdjatChestID)
            {
                uint8_t udjatChestX = static_cast<uint8_t>(std::round(entity->x));
                uint8_t udjatChestY = static_cast<uint8_t>(std::round(entity->y));
                doorSearchRect.x = udjatChestX - 5;
                doorSearchRect.width = 10;
                doorSearchRect.y = udjatChestY - 5;
                doorSearchRect.height = 1;
                Util::log(fmt::format("- FilterFindUdjatEye: found chest on {}-{} at x:{} y:{}", mCurrentWorld, mCurrentLevel, entity->x, entity->y));
                break;
            }
        }
        if (doorSearchRect.isValid())
        {
            for (Entity* entity : mBackLayerEntities)
            {
                auto eX = static_cast<uint8_t>(std::round(entity->x));
                auto eY = static_cast<uint8_t>(std::round(entity->y));

                if (entity->type->id == msLayerDoorID && doorSearchRect.containsPoint(eX, eY))
                {
                    mUdjatDoorX = eX;
                    mUdjatDoorY = eY;
                    Util::log(fmt::format("- FilterFindUdjatEye: found door on {}-{} at x:{} y:{}", mCurrentWorld, mCurrentLevel, entity->x, entity->y));
                    break;
                }
            }
        }
    }

    void FilterFindUdjatEye::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindUdjatEyeHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);
            FilterUI::renderAccessibilityChoice(&mKeyAccessibility, this, "Udjat key", "be accessible without bombs");
            FilterUI::renderDeleteFilterButton(this);

            ImGui::SetCursorPosX(gSectionMarginHor * 2);
            FilterUI::renderAccessibilityChoice(&mDoorAccessibility, this, "Udjat door", "be accessible without bombs");

            ImGui::SetCursorPosX(gSectionMarginHor * 2);
            FilterUI::renderPositionChoice(&mKeyPosition, this, "Udjat key should be", "compared to the Udjat door");

            FilterUI::renderLevelSelectorRadiobuttons(&mLevelsToSearch, true);
        }
    }

    void FilterFindUdjatEye::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindUdjatEye"));
        Util::log(fmt::format("\tKey accessibility: {}", mKeyAccessibility == AccessibilityChoice::MAYBE ? "ignored" : mKeyAccessibility == AccessibilityChoice::YES ? "yes" : "no"));
        Util::log(fmt::format("\tDoor accessibility: {}", mDoorAccessibility == AccessibilityChoice::MAYBE ? "ignored" : mDoorAccessibility == AccessibilityChoice::YES ? "yes" : "no"));
        Util::log(fmt::format("\tKey position: {}", mKeyPosition == PositionChoice::WHEREVER ? "wherever" : mKeyPosition == PositionChoice::HIGHER ? "higher than the door" : "lower than the door"));
        Util::log(fmt::format("\tLevel: {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }
} // namespace SeedFinder