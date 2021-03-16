#include "FilterFindLevelFeelings.h"
#include "SeedFinder.h"
#include "rpc.hpp"

namespace SeedFinder
{

    const char* FilterFindLevelFeelings::msComboItemOptions[] = {"", "Darkness", "Voice echoes", "Bee hive", "Dead are restless", "Metal Clanking", "Air of opression"};
    const char* FilterFindLevelFeelings::msComboItemNecessities[] = {"must have", "must not have"};

    FilterFindLevelFeelings::FilterFindLevelFeelings(SeedFinder* seedFinder) : Filter(seedFinder) {}

    std::string FilterFindLevelFeelings::title() { return "Find level feelings"; }

    uint8_t FilterFindLevelFeelings::deepestLevel() const { return mLevelsToSearch.deepest(); }

    bool FilterFindLevelFeelings::shouldExecute(uint8_t currentWorld, uint8_t currentLevel) { return mLevelsToSearch.shouldExecute(currentWorld, currentLevel); }

    bool FilterFindLevelFeelings::isValid()
    {
        setLastError("");
        auto levelsValid = mLevelsToSearch.isValid();
        if (!levelsValid)
        {
            setLastError("Please choose at least one level");
        }
        auto feelingValid = (mComboChosenFeeling != nullptr && mComboChosenFeeling != msComboItemOptions[0]);
        if (!feelingValid)
        {
            setLastError("Please choose a feeling");
        }

        auto feelingLevelSelectionValid = true;
        if (mComboChosenFeeling == msComboItemOptions[3]) // bee hive
        {
            if (mLevelsToSearch.OneOne || mLevelsToSearch.OneTwo || mLevelsToSearch.OneThree || mLevelsToSearch.Quillback || mLevelsToSearch.Olmec || mLevelsToSearch.IceCaves ||
                mLevelsToSearch.SixOne || mLevelsToSearch.SixTwo || mLevelsToSearch.SixThree || mLevelsToSearch.Tiamat || mLevelsToSearch.SevenOne || mLevelsToSearch.SevenTwo ||
                mLevelsToSearch.SevenThree || mLevelsToSearch.Hundun)
            {
                setLastError("Bee hives appear in the Jungle or the Temple only");
                feelingLevelSelectionValid = false;
            }

            if (!mSeedFinder->areWeGoingToJungle() && (mLevelsToSearch.TwoOne || mLevelsToSearch.TwoTwo || mLevelsToSearch.TwoThree || mLevelsToSearch.TwoFour))
            {
                setLastError("Bee hives are in the Jungle, can't go to Volcana (see configuration above)");
                feelingLevelSelectionValid = false;
            }

            if (!mSeedFinder->areWeGoingToTemple() && (mLevelsToSearch.FourOne || mLevelsToSearch.FourTwo || mLevelsToSearch.FourThree || mLevelsToSearch.FourFour))
            {
                setLastError("Bee hives are in the Temple, can't go to Tide Pool (see configuration above)");
                feelingLevelSelectionValid = false;
            }
        }
        else if (mComboChosenFeeling == msComboItemOptions[4]) // dead are restless
        {
            if (mLevelsToSearch.OneOne || mLevelsToSearch.OneTwo || mLevelsToSearch.OneThree || mLevelsToSearch.Quillback || mLevelsToSearch.Olmec || mLevelsToSearch.IceCaves ||
                mLevelsToSearch.SixOne || mLevelsToSearch.SixTwo || mLevelsToSearch.SixThree || mLevelsToSearch.Tiamat || mLevelsToSearch.SevenOne || mLevelsToSearch.SevenTwo ||
                mLevelsToSearch.SevenThree || mLevelsToSearch.Hundun)
            {
                setLastError("Dead are restless only in the Jungle, Volcana, Temple, or Tide Pool");
                feelingLevelSelectionValid = false;
            }
        }
        else if (mComboChosenFeeling == msComboItemOptions[5]) // metal clanking
        {
            if (mLevelsToSearch.OneOne || mLevelsToSearch.OneTwo || mLevelsToSearch.OneThree || mLevelsToSearch.Quillback || mLevelsToSearch.Olmec || mLevelsToSearch.FourOne ||
                mLevelsToSearch.FourTwo || mLevelsToSearch.FourThree || mLevelsToSearch.FourFour || mLevelsToSearch.IceCaves || mLevelsToSearch.SixOne || mLevelsToSearch.SixTwo ||
                mLevelsToSearch.SixThree || mLevelsToSearch.Tiamat || mLevelsToSearch.SevenOne || mLevelsToSearch.SevenTwo || mLevelsToSearch.SevenThree || mLevelsToSearch.Hundun)
            {
                setLastError("Metal clanks only in Volcana");
                feelingLevelSelectionValid = false;
            }
            if (!mSeedFinder->areWeGoingToVolcana() && (mLevelsToSearch.TwoOne || mLevelsToSearch.TwoTwo || mLevelsToSearch.TwoThree || mLevelsToSearch.TwoFour))
            {
                setLastError("Metal clanks only in Volcana, can't go to the Jungle (see configuration above)");
                feelingLevelSelectionValid = false;
            }
        }
        else if (mComboChosenFeeling == msComboItemOptions[6]) // air of oppression
        {
            if (mLevelsToSearch.OneOne || mLevelsToSearch.OneTwo || mLevelsToSearch.OneThree || mLevelsToSearch.Quillback || mLevelsToSearch.Olmec || mLevelsToSearch.TwoOne ||
                mLevelsToSearch.TwoTwo || mLevelsToSearch.TwoThree || mLevelsToSearch.TwoFour || mLevelsToSearch.IceCaves || mLevelsToSearch.SixOne || mLevelsToSearch.SixTwo ||
                mLevelsToSearch.SixThree || mLevelsToSearch.Tiamat || mLevelsToSearch.SevenOne || mLevelsToSearch.SevenTwo || mLevelsToSearch.SevenThree || mLevelsToSearch.Hundun)
            {
                setLastError("Air of oppression is only in Tide Pool");
                feelingLevelSelectionValid = false;
            }
            if (!mSeedFinder->areWeGoingToTidePool() && (mLevelsToSearch.FourOne || mLevelsToSearch.FourTwo || mLevelsToSearch.FourThree || mLevelsToSearch.FourFour))
            {
                setLastError("Air of oppression is only in Tide Pool, can't go to the Temple (see configuration above)");
                feelingLevelSelectionValid = false;
            }
        }
        return (levelsValid && feelingValid && feelingLevelSelectionValid);
    }

    bool FilterFindLevelFeelings::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        auto state = State::get();
        auto stateMemory = state.ptr();
        uint32_t flag = 0;

        if (mComboChosenFeeling == msComboItemOptions[1]) // darkness
        {
            // example seed B3167B59 on 1-2
            flag = (1 << 17);
        }
        else if (mComboChosenFeeling == msComboItemOptions[2]) // voice echoes
        {
            // example seed = D1BC51E3 level 1-2
            auto isEchoing = (stateMemory->w == 6 && stateMemory->h == 6);
            if (mComboChosenNecessity == msComboItemNecessities[0]) // must have
            {
                if (isEchoing == true)
                {
                    Util::log(fmt::format("- FilterFindLevelFeelings: {} {} on {}-{}", mComboChosenNecessity, mComboChosenFeeling, currentWorld, currentLevel));
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else // must not have
            {
                if (isEchoing == false)
                {
                    Util::log(fmt::format("- FilterFindLevelFeelings: {} {} on {}-{}", mComboChosenNecessity, mComboChosenFeeling, currentWorld, currentLevel));
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        else if (mComboChosenFeeling == msComboItemOptions[3]) // bee hive
        {
            // never seem to be bees on 2-2 or 4-2
            // example seed 0CE09997 on 2-4
            // example seed BABECDF4 on 4-1
            flag = (1 << 8);
        }
        else if (mComboChosenFeeling == msComboItemOptions[4]) // dead are restless
        {
            // example seed BDB269E3 on 2-1 Volcana
            flag = (1 << 7);
        }
        else if (mComboChosenFeeling == msComboItemOptions[5]) // metal clanking
        {
            // example seed B3090D18 on 2-1
            flag = (1 << 8);
        }
        else if (mComboChosenFeeling == msComboItemOptions[6]) // air of oppression
        {
            // example seed 804E20A5 on 4-1
            flag = (1 << 8);
        }
        else
        {
            return false;
        }

        auto result = false;
        if (mComboChosenNecessity == msComboItemNecessities[0]) // must have
        {
            result = ((stateMemory->hud_flags & flag) > 0);
        }
        else // must not have
        {
            result = ((stateMemory->hud_flags & flag) == 0);
        }

        if (result)
        {
            Util::log(fmt::format("- FilterFindLevelFeelings: {} {} on {}-{}", mComboChosenNecessity, mComboChosenFeeling, currentWorld, currentLevel));
        }

        return result;
    }

    void FilterFindLevelFeelings::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindLevelFeelingsHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Feeling: ");

            ImGui::SameLine();
            ImGui::PushItemWidth(150);
            if (ImGui::BeginCombo(fmt::format("##SeedFinderFilterFindLevelFeelingsNecessity{}", fmt::ptr(this)).c_str(), mComboChosenNecessity))
            {
                for (auto i = 0; i < 2; ++i)
                {
                    bool isSelected = (mComboChosenNecessity == msComboItemNecessities[i]);
                    if (ImGui::Selectable(msComboItemNecessities[i], isSelected))
                    {
                        mComboChosenNecessity = msComboItemNecessities[i];
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::SameLine();
            ImGui::PushItemWidth(200);
            if (ImGui::BeginCombo(fmt::format("##SeedFinderFilterFindLevelFeelingsFeeling{}", fmt::ptr(this)).c_str(), mComboChosenFeeling))
            {
                for (auto i = 0; i < cFeelingsCount; ++i)
                {
                    bool isSelected = (mComboChosenFeeling == msComboItemOptions[i]);
                    if (ImGui::Selectable(msComboItemOptions[i], isSelected))
                    {
                        mComboChosenFeeling = msComboItemOptions[i];
                    }
                }
                ImGui::EndCombo();
            }

            FilterUI::renderDeleteFilterButton(this);
            FilterUI::renderLevelSelectorCheckboxes(&mLevelsToSearch);
        }
    }

    void FilterFindLevelFeelings::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindLevelFeelings"));
        Util::log(fmt::format("\tFeeling: {}", mComboChosenFeeling));
        Util::log(fmt::format("\tLevel(s): {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }
} // namespace SeedFinder