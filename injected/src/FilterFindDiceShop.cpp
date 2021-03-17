#include "FilterFindDiceShop.h"
#include "SeedFinder.h"
#include "rpc.hpp"

namespace SeedFinder
{
    uint16_t FilterFindDiceShop::msDieID = 0;

    FilterFindDiceShop::FilterFindDiceShop(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msDieID == 0)
        {
            msDieID = static_cast<uint16_t>(to_id("ENT_TYPE_ITEM_DIE"));
        }
        mLevelsToSearch.disableLevel(1, 1);
        mLevelsToSearch.disableLevel(1, 4);
        mLevelsToSearch.disableLevel(3, 1);
        mLevelsToSearch.disableLevel(6, 4);
        mLevelsToSearch.disableLevel(7, 4);
    }

    std::string FilterFindDiceShop::uniqueIdentifier()
    {
        return "FilterFindDiceShop";
    }

    std::string FilterFindDiceShop::title()
    {
        return "Find dice shop";
    }

    std::unique_ptr<FilterFindDiceShop> FilterFindDiceShop::instantiate(SeedFinder* seedFinder)
    {
        return (std::make_unique<FilterFindDiceShop>(seedFinder));
    }

    uint8_t FilterFindDiceShop::deepestLevel() const
    {
        return mLevelsToSearch.deepest();
    }

    bool FilterFindDiceShop::shouldExecute(uint8_t currentWorld, uint8_t currentLevel)
    {
        return mLevelsToSearch.shouldExecute(currentWorld, currentLevel);
    }

    bool FilterFindDiceShop::isValid()
    {
        setLastError("");
        auto levelsValid = mLevelsToSearch.isValid();
        if (!levelsValid)
        {
            setLastError("Please choose at least one level");
        }
        return levelsValid;
    }

    bool FilterFindDiceShop::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        // example seed 600741A0 on 1-3
        auto state = State::get();
        auto entities = state.layer(0)->items();

        for (Entity* entity : entities)
        {
            if (entity->type->id == msDieID)
            {
                Util::log(fmt::format("- FilterFindDiceShop: found dice shop on {}-{} at x:{} y:{}", currentWorld, currentLevel, entity->x, entity->y));
                return true;
                break;
            }
        }
        return false;
    }

    void FilterFindDiceShop::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindDiceShopHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            // ImGui::SetCursorPosX(gSectionMarginHor * 2);
            ImGui::NewLine();

            FilterUI::renderDeleteFilterButton(this);
            FilterUI::renderLevelSelectorCheckboxes(&mLevelsToSearch);
        }
    }

    void FilterFindDiceShop::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindDiceShop"));
        Util::log(fmt::format("\tLevel(s): {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }

    json FilterFindDiceShop::serialize() const
    {
        json j;
        j[SeedFinder::kJSONVersion] = 1;
        j[SeedFinder::kJSONFilterID] = uniqueIdentifier();
        j[SeedFinder::kJSONLevels] = mLevelsToSearch.serialize();
        return j;
    }

    std::string FilterFindDiceShop::unserialize(const json& j)
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