#include "FilterFindBlackMarket.h"
#include "SeedFinder.h"

namespace SeedFinder
{
    uint16_t FilterFindBlackMarket::msHedjetID = 0;

    FilterFindBlackMarket::FilterFindBlackMarket(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msHedjetID == 0)
        {
            msHedjetID = static_cast<uint16_t>(to_id("ENT_TYPE_ITEM_PICKUP_HEDJET"));
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

    std::string FilterFindBlackMarket::uniqueIdentifier()
    {
        return "FilterFindBlackMarket";
    }

    std::string FilterFindBlackMarket::title()
    {
        return "Find black market";
    }

    std::unique_ptr<FilterFindBlackMarket> FilterFindBlackMarket::instantiate(SeedFinder* seedFinder)
    {
        return (std::make_unique<FilterFindBlackMarket>(seedFinder));
    }

    uint8_t FilterFindBlackMarket::deepestLevel() const
    {
        if (mLevelsToSearch.isValid())
        {
            return mLevelsToSearch.deepest();
        }
        else
        {
            return 7; // 2-4
        }
    }

    void FilterFindBlackMarket::resetForNewSeed(uint32_t newSeed)
    {
        mBlackMarketFound = false;
        mBlackMarketFoundOnLevel = 0;
    }

    bool FilterFindBlackMarket::shouldExecute(uint8_t currentWorld, uint8_t currentLevel)
    {
        if (currentWorld == 2 && (currentLevel == 2 || currentLevel == 3 || currentLevel == 4))
        {
            // see if there is a black market on this level
            mCurrentWorld = currentWorld;
            mCurrentLevel = currentLevel;
            if (!mBlackMarketFound)
            {
                locateBlackMarket();
            }

            // if levels were checked, return true if the provided one was checked; the execution will fail if the
            // black market isn't found anyway
            if (mLevelsToSearch.isValid())
            {
                return mLevelsToSearch.shouldExecute(currentWorld, currentLevel);
            }
            else
            {
                // in case no levels where checked, only execute when the black market has been found on this level
                return mBlackMarketFound;
            }
        }
        else
        {
            return false;
        }
    }

    bool FilterFindBlackMarket::isValid()
    {
        setLastError("");

        if (!mSeedFinder->areWeGoingToJungle())
        {
            setLastError("To find the black market, we must go to the Jungle instead of Volcana (in the global configuration above)");
            return false;
        }

        if (!mLevelsToSearch.isValid())
        {
            setLastError("Please choose which level the black market should appear on");
            return false;
        }
        return true;
    }

    bool FilterFindBlackMarket::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        mCurrentWorld = currentWorld;
        mCurrentLevel = currentLevel;

        if (!mBlackMarketFound)
        {
            return false;
        }

        if (mLevelsToSearch.isValid())
        {
            return ((mLevelsToSearch.TwoTwo && mBlackMarketFoundOnLevel == 2) || (mLevelsToSearch.TwoThree && mBlackMarketFoundOnLevel == 3) ||
                    (mLevelsToSearch.TwoFour && mBlackMarketFoundOnLevel == 4));
        }
        else
        {
            return true;
        }
    }

    void FilterFindBlackMarket::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindBlackMarketHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            FilterUI::renderLevelSelectorRadiobuttons(&mLevelsToSearch);
            FilterUI::renderDeleteFilterButton(this);
        }
    }

    void FilterFindBlackMarket::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindBlackMarket"));
        if (mLevelsToSearch.isValid())
        {
            Util::log(fmt::format("\tLevel(s): {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
        }
        else
        {
            Util::log("\tLevel(s): 2-2 or 2-3 or 2-4");
        }
    }

    void FilterFindBlackMarket::locateBlackMarket()
    {
        auto state = State::get();
        mFrontLayerEntities = state.layer(1)->items();
        for (Entity* entity : mFrontLayerEntities)
        {
            if (entity->type->id == msHedjetID)
            {
                mBlackMarketFound = true;
                mBlackMarketFoundOnLevel = mCurrentLevel;
                Util::log(fmt::format("- FilterFindBlackMarket: found black market on {}-{}", mCurrentWorld, mCurrentLevel));
                break;
            }
        }
    }

    json FilterFindBlackMarket::serialize() const
    {
        json j;
        j[SeedFinder::kJSONVersion] = 1;
        j[SeedFinder::kJSONFilterID] = uniqueIdentifier();
        j[SeedFinder::kJSONLevels] = mLevelsToSearch.serialize();
        return j;
    }

    std::string FilterFindBlackMarket::unserialize(const json& j)
    {
        if (j.contains(SeedFinder::kJSONVersion))
        {
            auto version = j.at(SeedFinder::kJSONVersion).get<uint8_t>();
            if (version == 1)
            {
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