#include "FilterFindVolcanaDrill.h"
#include "AStar/SimplifiedLevel.h"
#include "SeedFinder.h"

namespace SeedFinder
{
    uint16_t FilterFindVolcanaDrill::msDrillIndicatorID = 0;
    uint16_t FilterFindVolcanaDrill::msStagnantLavaID = 0;
    uint16_t FilterFindVolcanaDrill::msLiquidLavaID = 0;
    uint16_t FilterFindVolcanaDrill::msAltarID = 0;
    uint16_t FilterFindVolcanaDrill::msShopBGID = 0;
    const char* FilterFindVolcanaDrill::msComparisonOptions[] = {"less than or equal to", "greater than"};

    FilterFindVolcanaDrill::FilterFindVolcanaDrill(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msDrillIndicatorID == 0)
        {
            msDrillIndicatorID = static_cast<uint16_t>(to_id("ENT_TYPE_BG_DRILL_INDICATOR"));
            msStagnantLavaID = static_cast<uint16_t>(to_id("ENT_TYPE_LIQUID_STAGNANT_LAVA"));
            msLiquidLavaID = static_cast<uint16_t>(to_id("ENT_TYPE_LIQUID_LAVA"));
            msAltarID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_ALTAR"));
            msShopBGID = static_cast<uint16_t>(to_id("ENT_TYPE_BG_SHOP"));
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
        mLevelsToSearch.Any = true;
    }

    std::string FilterFindVolcanaDrill::title() { return "Find Volcana drill"; }

    uint8_t FilterFindVolcanaDrill::deepestLevel() const
    {
        if (mLevelsToSearch.Any)
        {
            return 7; // 2-4
        }
        else
        {
            return mLevelsToSearch.deepest();
        }
    }

    void FilterFindVolcanaDrill::resetForNewSeed(uint32_t newSeed)
    {
        mDrillFound = false;
        mDrillFoundOnLevel = 0;
    }

    bool FilterFindVolcanaDrill::shouldExecute(uint8_t currentWorld, uint8_t currentLevel)
    {
        if (currentWorld == 2 && (currentLevel == 2 || currentLevel == 3 || currentLevel == 4))
        {
            mCurrentWorld = currentWorld;
            mCurrentLevel = currentLevel;

            // see if there is a drill on this level
            if (!mDrillFound)
            {
                locateDrill();
            }

            if (mLevelsToSearch.Any)
            {
                // if 'Any', only execute when the drill is actually here, otherwise the two other levels
                // would execute and return false, making it so the seed won't get picked
                return mDrillFoundOnLevel == currentLevel;
            }
            else
            {
                // if a specific level was forced, it must be executed, so that the seed won't get picked
                // if the drill isn't there
                return mLevelsToSearch.shouldExecute(currentWorld, currentLevel);
            }
        }
        else
        {
            return false;
        }
    }

    bool FilterFindVolcanaDrill::isValid()
    {
        setLastError("");

        if (!mSeedFinder->areWeGoingToVolcana())
        {
            setLastError("To find the drill, we must go to Volcana instead of the Jungle (in the global configuration above)");
            return false;
        }

        return true;
    }

    bool FilterFindVolcanaDrill::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        mCurrentWorld = currentWorld;
        mCurrentLevel = currentLevel;

        if (!mDrillFound || mDrillFoundOnLevel != currentLevel)
        {
            return false;
        }

        bool levelSatisfied = mLevelsToSearch.Any ||
                              ((mLevelsToSearch.TwoTwo && mDrillFoundOnLevel == 2) || (mLevelsToSearch.TwoThree && mDrillFoundOnLevel == 3) || (mLevelsToSearch.TwoFour && mDrillFoundOnLevel == 4));

        if (!levelSatisfied)
        {
            return false;
        }

        auto state = State::get();
        const uint8_t lava = 'L';
        const uint8_t shop = 'S';
        const uint8_t altar = 'A';
        auto simpleLevel =
            SimplifiedLevel(mSeedFinder, state.ptr()->w, state.ptr()->h, mFrontLayerEntities, false, {{msLiquidLavaID, lava}, {msStagnantLavaID, lava}, {msShopBGID, shop}, {msAltarID, altar}});

        // Count lava under drill
        bool lavaCountSatisfied;
        uint16_t totalLavaCount = 0;
        auto levelWidth = simpleLevel.width();

        for (auto y = SimplifiedLevel::LEVEL_HEIGHT; y >= SimplifiedLevel::LEVEL_HEIGHT - simpleLevel.height(); --y)
        {
            // first, straight down, check the two blocks directly underneath the drill
            if (simpleLevel.blockValue(mDrillX, y) == lava)
            {
                totalLavaCount++;
            }
            if (simpleLevel.blockValue(mDrillX + 1, y) == lava)
            {
                totalLavaCount++;
            }

            // second, the connected blocks to the left of the drill path
            for (auto x = mDrillX - 1; x >= 0; --x)
            {
                auto blockValue = simpleLevel.blockValue(x, y);
                if (blockValue == lava)
                {
                    totalLavaCount++;
                }
                else
                {
                    break;
                }
            }

            // third, the connected blocks to the right of the drill path
            for (auto x = mDrillX + 2; x < levelWidth; ++x)
            {
                auto blockValue = simpleLevel.blockValue(x, y);
                if (blockValue == lava)
                {
                    totalLavaCount++;
                }
                else
                {
                    break;
                }
            }
        }

        Util::log(fmt::format("- FilterFindVolcanaDrill: found {} lava block(s) in the drill path", totalLavaCount));
        if (mChosenComparison == msComparisonOptions[0]) // less than or equal to
        {
            lavaCountSatisfied = (totalLavaCount <= mAmountOfLavaBlocks);
        }
        else
        {
            lavaCountSatisfied = (totalLavaCount > mAmountOfLavaBlocks);
        }

        if (!lavaCountSatisfied)
        {
            return false;
        }

        // Test shop under drill
        bool shopSatisfied = true;
        if (mShopUnderDrill != AccessibilityChoice::MAYBE)
        {
            bool shopFound = false;
            for (auto y = SimplifiedLevel::LEVEL_HEIGHT; y >= SimplifiedLevel::LEVEL_HEIGHT - simpleLevel.height(); --y)
            {
                if (simpleLevel.blockValue(mDrillX, y) == shop || simpleLevel.blockValue(mDrillX + 1, y) == shop)
                {
                    shopFound = true;
                    break;
                }
            }
            Util::log(fmt::format("- FilterFindVolcanaDrill: shop in the drill path: {}", shopFound));
            if (mShopUnderDrill == AccessibilityChoice::YES)
            {
                shopSatisfied = (shopFound == true);
            }
            else
            {
                shopSatisfied = (shopFound == false);
            }
        }
        if (!shopSatisfied)
        {
            return false;
        }

        // Test altar under drill
        bool altarSatisfied = true;
        if (mAltarUnderDrill != AccessibilityChoice::MAYBE)
        {
            bool altarFound = false;
            for (auto y = SimplifiedLevel::LEVEL_HEIGHT; y >= SimplifiedLevel::LEVEL_HEIGHT - simpleLevel.height(); --y)
            {
                if (simpleLevel.blockValue(mDrillX, y) == altar || simpleLevel.blockValue(mDrillX + 1, y) == altar)
                {
                    altarFound = true;
                    break;
                }
            }
            Util::log(fmt::format("- FilterFindVolcanaDrill: altar in the drill path: {}", altarFound));
            if (mAltarUnderDrill == AccessibilityChoice::YES)
            {
                altarSatisfied = (altarFound == true);
            }
            else
            {
                altarSatisfied = (altarFound == false);
            }
        }
        if (!altarSatisfied)
        {
            return false;
        }

        return (levelSatisfied && lavaCountSatisfied && shopSatisfied && altarSatisfied);
    }

    void FilterFindVolcanaDrill::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindVolcanaDrillHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);

            ImGui::AlignTextToFramePadding();
            ImGui::Text("Amount of lava under the drill should be");
            ImGui::SameLine();

            ImGui::PushItemWidth(160);
            if (ImGui::BeginCombo(fmt::format("##SeedFinderFilterFindVolcanaDrillPrefix1{}", fmt::ptr(this)).c_str(), mChosenComparison))
            {
                for (auto i = 0; i < IM_ARRAYSIZE(msComparisonOptions); ++i)
                {
                    bool isSelected = (mChosenComparison == msComparisonOptions[i]);
                    if (ImGui::Selectable(msComparisonOptions[i], isSelected))
                    {
                        mChosenComparison = msComparisonOptions[i];
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();

            ImGui::PushItemWidth(50);
            ImGui::InputScalar("##SeedFinderFilterFindVolcanaDrillAmountOfLavaBlocks", ImGuiDataType_U16, &mAmountOfLavaBlocks);

            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("block(s)");

            FilterUI::renderDeleteFilterButton(this);

            ImGui::SetCursorPosX(gSectionMarginHor * 2);
            FilterUI::renderAccessibilityChoice(&mShopUnderDrill, this, "Shop", "be under the drill");

            ImGui::SetCursorPosX(gSectionMarginHor * 2);
            FilterUI::renderAccessibilityChoice(&mAltarUnderDrill, this, "Altar", "be under the drill");

            FilterUI::renderLevelSelectorRadiobuttons(&mLevelsToSearch, true);
        }
    }

    void FilterFindVolcanaDrill::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindVolcanaDrill"));
        Util::log(fmt::format("\tLava under drill: {} {} block(s)", mChosenComparison, mAmountOfLavaBlocks));
        Util::log(fmt::format("\tShop under drill: {}", mShopUnderDrill == AccessibilityChoice::MAYBE ? "ignored" : mShopUnderDrill == AccessibilityChoice::YES ? "yes" : "no"));
        Util::log(fmt::format("\tAltar under drill: {}", mAltarUnderDrill == AccessibilityChoice::MAYBE ? "ignored" : mAltarUnderDrill == AccessibilityChoice::YES ? "yes" : "no"));
        Util::log(fmt::format("\tLevel: {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }

    void FilterFindVolcanaDrill::locateDrill()
    {
        mDrillX = 0;
        auto state = State::get();
        mFrontLayerEntities = state.layer(0)->items();
        for (Entity* entity : mFrontLayerEntities)
        {
            if (entity->type->id == msDrillIndicatorID)
            {
                mDrillX = static_cast<uint8_t>(std::floor(entity->x));
                mDrillFound = true;
                mDrillFoundOnLevel = mCurrentLevel;
                Util::log(fmt::format("- FilterFindVolcanaDrill: found drill on {}-{} at x:{} y:{}", mCurrentWorld, mCurrentLevel, entity->x, entity->y));
                break;
            }
        }
    }

} // namespace SeedFinder