#include "FilterFindMoonChallenge.h"
#include "AStar/SimplifiedLevel.h"
#include "SeedFinder.h"

namespace SeedFinder
{
    uint16_t FilterFindMoonChallenge::msTunID = 0;
    uint16_t FilterFindMoonChallenge::msMattockID = 0;
    const char* FilterFindMoonChallenge::msComparisonOptions[] = {"less than or equal to", "greater than"};
    const char* FilterFindMoonChallenge::kJSONMattockDurability = "mattock_durability";
    const char* FilterFindMoonChallenge::kJSONMattockComparison = "mattock_comparison";

    FilterFindMoonChallenge::FilterFindMoonChallenge(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msTunID == 0)
        {
            msTunID = static_cast<uint16_t>(to_id("ENT_TYPE_MONS_MERCHANT"));
            msMattockID = static_cast<uint16_t>(to_id("ENT_TYPE_ITEM_MATTOCK"));
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

    std::string FilterFindMoonChallenge::uniqueIdentifier()
    {
        return "FilterFindMoonChallenge";
    }

    std::string FilterFindMoonChallenge::title()
    {
        return "Find Moon challenge";
    }

    std::unique_ptr<FilterFindMoonChallenge> FilterFindMoonChallenge::instantiate(SeedFinder* seedFinder)
    {
        return (std::make_unique<FilterFindMoonChallenge>(seedFinder));
    }

    uint8_t FilterFindMoonChallenge::deepestLevel() const
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

    void FilterFindMoonChallenge::resetForNewSeed(uint32_t newSeed)
    {
        mChallengeFound = false;
        mChallengeFoundOnLevel = 0;
    }

    bool FilterFindMoonChallenge::shouldExecute(uint8_t currentWorld, uint8_t currentLevel)
    {
        if (currentWorld == 2 && (currentLevel == 2 || currentLevel == 3 || currentLevel == 4))
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

    bool FilterFindMoonChallenge::isValid()
    {
        return true;
    }

    bool FilterFindMoonChallenge::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        mCurrentWorld = currentWorld;
        mCurrentLevel = currentLevel;

        if (!mChallengeFound || mChallengeFoundOnLevel != currentLevel)
        {
            return false;
        }

        auto levelSatisfied = mLevelsToSearch.Any || ((mLevelsToSearch.TwoTwo && mChallengeFoundOnLevel == 2) || (mLevelsToSearch.TwoThree && mChallengeFoundOnLevel == 3) ||
                                                      (mLevelsToSearch.TwoFour && mChallengeFoundOnLevel == 4));

        if (!levelSatisfied)
        {
            return false;
        }

        auto accessibilitySatisfied = false;
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
            Util::log(fmt::format("- FilterFindMoonChallenge: moon challenge accessible: {}", accessible));

            if (mAccessibility == AccessibilityChoice::YES)
            {
                accessibilitySatisfied = (accessible == true);
            }
            if (mAccessibility == AccessibilityChoice::NO)
            {
                accessibilitySatisfied = (accessible == false);
            }
        }

        if (!accessibilitySatisfied)
        {
            return false;
        }

        auto mattockDurabilitySatisfied = false;
        auto state = State::get();
        auto backLayerEntities = state.layer(1)->all_entities.entities();
        for (Entity* entity : backLayerEntities)
        {
            if (entity->type->id == msMattockID)
            {
                auto mattock = entity->as<Mattock>();
                Util::log(fmt::format("- FilterFindMoonChallenge: found mattock at x:{} y:{} with durability {}", entity->x, entity->y, mattock->remaining));
                if (mChosenMattockComparison == msComparisonOptions[0]) // less than or equal to
                {
                    mattockDurabilitySatisfied = (mattock->remaining <= mMinimumMattockDurability);
                }
                else
                {
                    mattockDurabilitySatisfied = (mattock->remaining > mMinimumMattockDurability);
                }
                break;
            }
        }

        return (levelSatisfied && accessibilitySatisfied && mattockDurabilitySatisfied);
    }

    void FilterFindMoonChallenge::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindMoonChallengeHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);

            ImGui::AlignTextToFramePadding();
            ImGui::Text("Mattock durability should be");
            ImGui::SameLine();

            ImGui::PushItemWidth(160);
            if (ImGui::BeginCombo(fmt::format("##SeedFinderFilterFindVolcanaDrillPrefix1{}", fmt::ptr(this)).c_str(), mChosenMattockComparison))
            {
                for (auto i = 0; i < IM_ARRAYSIZE(msComparisonOptions); ++i)
                {
                    bool isSelected = (mChosenMattockComparison == msComparisonOptions[i]);
                    if (ImGui::Selectable(msComparisonOptions[i], isSelected))
                    {
                        mChosenMattockComparison = msComparisonOptions[i];
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();

            ImGui::PushItemWidth(50);
            ImGui::InputScalar("##SeedFinderFilterFindVolcanaDrillAmountOfLavaBlocks", ImGuiDataType_U8, &mMinimumMattockDurability);

            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("[40 ~ 60]");

            FilterUI::renderDeleteFilterButton(this);

            ImGui::SetCursorPosX(gSectionMarginHor * 2);
            FilterUI::renderAccessibilityChoice(&mAccessibility, this, "Moon challenge", "be accessible without bombs");

            FilterUI::renderLevelSelectorRadiobuttons(&mLevelsToSearch, true);
        }
    }

    void FilterFindMoonChallenge::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindMoonChallenge"));
        Util::log(fmt::format("\tAccessible: {}", mAccessibility == AccessibilityChoice::MAYBE ? "ignored" : mAccessibility == AccessibilityChoice::YES ? "yes" : "no"));
        Util::log(fmt::format("\tLevel: {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }

    void FilterFindMoonChallenge::locateTun()
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
                Util::log(fmt::format("- FilterFindMoonChallenge: found Tun on {}-{} at x:{} y:{}", mCurrentWorld, mCurrentLevel, entity->x, entity->y));
                break;
            }
        }
    }

    json FilterFindMoonChallenge::serialize() const
    {
        json j;
        j[SeedFinder::kJSONVersion] = 1;
        j[SeedFinder::kJSONFilterID] = uniqueIdentifier();
        j[SeedFinder::kJSONAccessibility] = static_cast<int>(mAccessibility);
        j[kJSONMattockDurability] = mMinimumMattockDurability;
        j[kJSONMattockComparison] = mChosenMattockComparison;
        j[SeedFinder::kJSONLevels] = mLevelsToSearch.serialize();
        return j;
    }

    std::string FilterFindMoonChallenge::unserialize(const json& j)
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
                if (j.contains(kJSONMattockDurability))
                {
                    mMinimumMattockDurability = j.at(kJSONMattockDurability).get<uint8_t>();
                }
                if (j.contains(kJSONMattockComparison))
                {
                    auto comparison = j.at(kJSONMattockComparison).get<std::string>();
                    mChosenMattockComparison = msComparisonOptions[0];
                    for (auto x = 0; x < 2; ++x)
                    {
                        if (comparison == msComparisonOptions[x])
                        {
                            mChosenMattockComparison = msComparisonOptions[x];
                            break;
                        }
                    }
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