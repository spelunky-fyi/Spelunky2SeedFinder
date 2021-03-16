#include "FilterFindEntity.h"
#include "SeedFinder.h"

namespace SeedFinder
{

    FilterFindEntity::FilterFindEntity(SeedFinder* seedFinder) : Filter(seedFinder) {}

    std::string FilterFindEntity::title() { return "Find entity"; }

    uint8_t FilterFindEntity::deepestLevel() const { return mLevelsToSearch.deepest(); }

    bool FilterFindEntity::shouldExecute(uint8_t currentWorld, uint8_t currentLevel) { return mLevelsToSearch.shouldExecute(currentWorld, currentLevel); }

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
        const auto checkLayer = [&](const std::vector<Entity*>& items) {
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
            checkLayer(state.layer(0)->items());
        }
        if (mLayer == LayerChoice::BACK || mLayer == LayerChoice::ALL)
        {
            checkLayer(state.layer(1)->items());
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
} // namespace SeedFinder