#include "FilterFindEntity.h"
#include "SeedFinder.h"

namespace SeedFinder
{
    FilterFindEntity::FilterFindEntity(SeedFinder* seedFinder) : Filter(seedFinder) {}

    std::string FilterFindEntity::uniqueIdentifier()
    {
        return "FilterFindEntity";
    }

    std::string FilterFindEntity::title()
    {
        return "Find entity";
    }

    std::unique_ptr<FilterFindEntity> FilterFindEntity::instantiate(SeedFinder* seedFinder)
    {
        return (std::make_unique<FilterFindEntity>(seedFinder));
    }

    uint8_t FilterFindEntity::deepestLevel() const
    {
        return mLevelsToSearch.deepest();
    }

    bool FilterFindEntity::shouldExecute(uint8_t currentWorld, uint8_t currentLevel)
    {
        return mLevelsToSearch.shouldExecute(currentWorld, currentLevel);
    }

    bool FilterFindEntity::isValid()
    {
        setLastError("");
        auto levelsValid = mLevelsToSearch.isValid();
        if (!levelsValid)
        {
            setLastError("Please choose at least one level");
        }
        if (mItemID == 0)
        {
            setLastError("Please choose an item");
        }

        return (levelsValid && mItemID != 0);
    }

    bool FilterFindEntity::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        auto state = State::get();

        uint8_t foundAmount = 0;
        const auto checkLayer = [&](const EntityList::EntityRange& items) {
            for (auto item : items)
            {
                if (item->type->id == mItemID)
                {
                    Util::log(fmt::format("- FilterFindEntity: found entity {} on {}-{} at x:{} y:{}", mItemID, currentWorld, currentLevel, item->x, item->y));
                    foundAmount++;
                    if (foundAmount >= mMinimumAmount)
                    {
                        Util::log(fmt::format("- FilterFindEntity: minimum amount of entity {} on {}-{} satisfied", mItemID, currentWorld, currentLevel));
                        return;
                    }
                }
            }
        };

        if (mLayer == LayerChoice::FRONT || mLayer == LayerChoice::ALL)
        {
            checkLayer(state.layer(0)->all_entities.entities());
        }
        if (mLayer == LayerChoice::BACK || mLayer == LayerChoice::ALL)
        {
            checkLayer(state.layer(1)->all_entities.entities());
        }
        return foundAmount >= mMinimumAmount;
    }

    void FilterFindEntity::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindEntityHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);

            ImGui::AlignTextToFramePadding();
            ImGui::Text("Item :");
            ImGui::SameLine();
            ImGui::PushItemWidth(300);
            if (ImGui::BeginCombo(fmt::format("##SeedFinderFilterFindEntityItemID{}", fmt::ptr(this)).c_str(), mComboChosenItem))
            {
                for (const auto& entity : mSeedFinder->allEntities())
                {
                    bool isSelected = (mItemID == entity.id);
                    if (ImGui::Selectable(fmt::format("{:03}: {}##SeedFinderFilterEntity{}{}", entity.id, entity.name.substr(9), entity.id, fmt::ptr(this)).c_str(), isSelected))
                    {
                        mComboChosenItem = entity.name.c_str();
                        mItemID = entity.id;
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Min. amount :");
            ImGui::SameLine();
            ImGui::PushItemWidth(30);
            ImGui::InputScalar(fmt::format("##SeedFinderFilterFindEntityMinimumAmount{}", fmt::ptr(this)).c_str(), ImGuiDataType_U8, &mMinimumAmount);

            ImGui::SameLine();
            FilterUI::renderLayerSelector(&mLayer, this);
            FilterUI::renderDeleteFilterButton(this);
            FilterUI::renderLevelSelectorCheckboxes(&mLevelsToSearch);
        }
    }

    void FilterFindEntity::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindEntity"));
        Util::log(fmt::format("\tItem ID: {} ({})", mItemID, mSeedFinder->entityName(mItemID)));
        Util::log(fmt::format("\tMinimum amount: {}", mMinimumAmount));
        Util::log(fmt::format("\tLayer: {}", mLayer == LayerChoice::ALL ? "All" : mLayer == LayerChoice::FRONT ? "Front" : "Back"));
        Util::log(fmt::format("\tLevel(s): {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }

    json FilterFindEntity::serialize() const
    {
        json j;
        j[SeedFinder::kJSONVersion] = 1;
        j[SeedFinder::kJSONFilterID] = uniqueIdentifier();
        j[SeedFinder::kJSONItem] = mSeedFinder->entityName(mItemID);
        j[SeedFinder::kJSONLayer] = static_cast<int>(mLayer);
        j[SeedFinder::kJSONMinimum] = mMinimumAmount;
        j[SeedFinder::kJSONLevels] = mLevelsToSearch.serialize();
        return j;
    }

    std::string FilterFindEntity::unserialize(const json& j)
    {
        if (j.contains(SeedFinder::kJSONVersion))
        {
            auto version = j.at(SeedFinder::kJSONVersion).get<uint8_t>();
            if (version == 1)
            {
                if (j.contains(SeedFinder::kJSONItem))
                {
                    mItemID = to_id(j.at(SeedFinder::kJSONItem).get<std::string>());
                    for (const auto& entity : mSeedFinder->allEntities())
                    {
                        if (entity.id == mItemID)
                        {
                            mComboChosenItem = entity.name.c_str();
                            break;
                        }
                    }
                }
                if (j.contains(SeedFinder::kJSONLayer))
                {
                    mLayer = static_cast<LayerChoice>(j.at(SeedFinder::kJSONLayer).get<uint8_t>());
                }
                if (j.contains(SeedFinder::kJSONMinimum))
                {
                    mMinimumAmount = j.at(SeedFinder::kJSONMinimum).get<uint8_t>();
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