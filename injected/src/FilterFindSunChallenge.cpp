#include "FilterFindSunChallenge.h"
#include "AStar/SimplifiedLevel.h"
#include "SeedFinder.h"

namespace SeedFinder
{
    uint16_t FilterFindSunChallenge::msTunID = 0;

    FilterFindSunChallenge::FilterFindSunChallenge(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msTunID == 0)
        {
            msTunID = static_cast<uint16_t>(to_id("ENT_TYPE_MONS_MERCHANT"));
        }
        mLevelsToSearch.hideLevel(1, 1);
        mLevelsToSearch.hideLevel(1, 2);
        mLevelsToSearch.hideLevel(1, 3);
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
        mLevelsToSearch.hideLevel(7, 4);
        mLevelsToSearch.Any = true;
    }

    std::string FilterFindSunChallenge::uniqueIdentifier()
    {
        return "FilterFindSunChallenge";
    }

    std::string FilterFindSunChallenge::title()
    {
        return "Find Sun challenge";
    }

    std::unique_ptr<FilterFindSunChallenge> FilterFindSunChallenge::instantiate(SeedFinder* seedFinder)
    {
        return (std::make_unique<FilterFindSunChallenge>(seedFinder));
    }

    uint8_t FilterFindSunChallenge::deepestLevel() const
    {
        if (mLevelsToSearch.Any)
        {
            return 20; // 7-3
        }
        else
        {
            return mLevelsToSearch.deepest();
        }
    }

    void FilterFindSunChallenge::resetForNewSeed(uint32_t newSeed)
    {
        mChallengeFound = false;
        mChallengeFoundOnLevel = 0;
    }

    bool FilterFindSunChallenge::shouldExecute(uint8_t currentWorld, uint8_t currentLevel)
    {
        if (currentWorld == 7 && (currentLevel == 1 || currentLevel == 2 || currentLevel == 3))
        {
            mCurrentWorld = currentWorld;
            mCurrentLevel = currentLevel;

            // see if Tun is on this level
            if (!mChallengeFound)
            {
                locateTun();
            }

            if (mLevelsToSearch.Any)
            {
                // if 'Any', only execute when Tun is actually here, otherwise the two other levels
                // would execute and return false, making it so the seed won't get picked
                return mChallengeFoundOnLevel == currentLevel;
            }
            else
            {
                // if a specific level was forced, it must be executed, so that the seed won't get picked
                // if Tun isn't there
                return mLevelsToSearch.shouldExecute(currentWorld, currentLevel);
            }
        }
        else
        {
            return false;
        }
    }

    bool FilterFindSunChallenge::isValid()
    {
        return true;
    }

    bool FilterFindSunChallenge::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        mCurrentWorld = currentWorld;
        mCurrentLevel = currentLevel;

        if (!mChallengeFound || mChallengeFoundOnLevel != currentLevel)
        {
            return false;
        }

        bool levelSatisfied = mLevelsToSearch.Any || ((mLevelsToSearch.SevenOne && mChallengeFoundOnLevel == 1) || (mLevelsToSearch.SevenTwo && mChallengeFoundOnLevel == 2) ||
                                                      (mLevelsToSearch.SevenThree && mChallengeFoundOnLevel == 3));

        if (!levelSatisfied)
        {
            return false;
        }

        bool accessibilitySatisfied = false;

        if (mAccessibility == AccessibilityChoice::MAYBE)
        {
            accessibilitySatisfied = true;
        }
        else
        {
            // locate the start door
            uint8_t startDoorX = 0;
            uint8_t startDoorY = 0;
            for (Entity* entity : mFrontLayerEntities)
            {
                if (entity->type->id == Filter::msStartDoorID)
                {
                    startDoorX = static_cast<uint8_t>(std::round(entity->x));
                    startDoorY = static_cast<uint8_t>(std::round(entity->y));
                    break;
                }
            }

            // see if Tun is connected to the start door
            auto state = State::get();
            auto simpleLevel = SimplifiedLevel(mSeedFinder, state.ptr()->w, state.ptr()->h, mFrontLayerEntities);
            bool accessible = simpleLevel.areTilesConnected(startDoorX, startDoorY, mTunX, mTunY);
            Util::log(fmt::format("- FilterFindSunChallenge: Sun challenge accessible: {}", accessible));

            if (mAccessibility == AccessibilityChoice::YES)
            {
                accessibilitySatisfied = (accessible == true);
            }
            if (mAccessibility == AccessibilityChoice::NO)
            {
                accessibilitySatisfied = (accessible == false);
            }
        }

        return (levelSatisfied && accessibilitySatisfied);
    }

    void FilterFindSunChallenge::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindSunChallengeHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);

            FilterUI::renderAccessibilityChoice(&mAccessibility, this, "Sun challenge", "be accessible without bombs");
            FilterUI::renderDeleteFilterButton(this);

            FilterUI::renderLevelSelectorRadiobuttons(&mLevelsToSearch, true);
        }
    }

    void FilterFindSunChallenge::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindSunChallenge"));
        Util::log(fmt::format("\tAccessible: {}", mAccessibility == AccessibilityChoice::MAYBE ? "ignored" : mAccessibility == AccessibilityChoice::YES ? "yes" : "no"));
        Util::log(fmt::format("\tLevel: {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }

    void FilterFindSunChallenge::locateTun()
    {
        mTunX = 0;
        mTunY = 0;
        auto state = State::get();
        mFrontLayerEntities = state.layer(0)->all_entities.entities();
        for (Entity* entity : mFrontLayerEntities)
        {
            if (entity->type->id == msTunID)
            {
                mTunX = static_cast<uint8_t>(std::floor(entity->x));
                mTunY = static_cast<uint8_t>(std::floor(entity->y));
                mChallengeFound = true;
                mChallengeFoundOnLevel = mCurrentLevel;
                Util::log(fmt::format("- FilterFindSunChallenge: found Tun on {}-{} at x:{} y:{}", mCurrentWorld, mCurrentLevel, entity->x, entity->y));
                break;
            }
        }
    }

    json FilterFindSunChallenge::serialize() const
    {
        json j;
        j[SeedFinder::kJSONVersion] = 1;
        j[SeedFinder::kJSONFilterID] = uniqueIdentifier();
        j[SeedFinder::kJSONAccessibility] = static_cast<int>(mAccessibility);
        j[SeedFinder::kJSONLevels] = mLevelsToSearch.serialize();
        return j;
    }

    std::string FilterFindSunChallenge::unserialize(const json& j)
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