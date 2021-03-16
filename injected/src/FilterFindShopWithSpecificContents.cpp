#include "FilterFindShopWithSpecificContents.h"
#include "SeedFinder.h"

namespace SeedFinder
{
    size_t FilterFindShopWithSpecificContents::msBGShopID = 0;
    size_t FilterFindShopWithSpecificContents::msPresentID = 0;

    FilterFindShopWithSpecificContents::FilterFindShopWithSpecificContents(SeedFinder* seedFinder) : Filter(seedFinder)
    {
        if (msBGShopID == 0)
        {
            msBGShopID = to_id("ENT_TYPE_BG_SHOP");
            msPresentID = to_id("ENT_TYPE_ITEM_PRESENT");
        }
    }

    uint8_t FilterFindShopWithSpecificContents::deepestLevel() const { return mLevelsToSearch.deepest(); }

    bool FilterFindShopWithSpecificContents::shouldExecute(uint8_t currentWorld, uint8_t currentLevel) { return mLevelsToSearch.shouldExecute(currentWorld, currentLevel); }

    bool FilterFindShopWithSpecificContents::isValid()
    {
        setLastError("");
        auto levelsValid = mLevelsToSearch.isValid();
        if (!levelsValid)
        {
            setLastError("Please choose at least one level");
        }

        bool itemIDsEntered = false;
        for (auto x = 0; x < itemPositionsInShop(); ++x)
        {
            if (mItemIDs[x] != 0)
            {
                itemIDsEntered = true;
            }
        }

        if (!itemIDsEntered)
        {
            setLastError("Please choose the item(s) to appear in the shop");
        }

        return (levelsValid && itemIDsEntered);
    }

    bool FilterFindShopWithSpecificContents::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        auto state = State::get();
        const auto checkForMatchingShops = [&](const std::vector<Entity*>& entities) {
            for (const Entity* shopkeeperEntity : entities)
            {
                if (shopkeeperEntity->type->id == shopKeeperID())
                {
                    auto searchRect = Rect(shopkeeperEntity->x - 5.5f, shopkeeperEntity->y - 0.5f, 5, 2);
                    if ((shopkeeperEntity->flags & 0x10000) == 0) // flag 0x10000 (bit 16) == 0 -> facing right
                    {
                        searchRect = Rect(shopkeeperEntity->x, shopkeeperEntity->y - 0.5f, 5.5f, 2);
                    }
                    auto foundEntities = mSeedFinder->entitiesInRect(searchRect, entities);
                    bool shopFound = false;
                    std::vector<uint32_t> shopItems = {};
                    for (Entity* e : foundEntities)
                    {
                        if (e->type->id == msBGShopID)
                        {
                            shopFound = true;
                        }
                        for (auto i = 0; i < itemPossibilitiesInShop(); ++i)
                        {
                            auto itemID = shopItemID(i);
                            if (itemID != msPresentID && e->type->id == msPresentID)
                            {
                                auto container = e->as<Container>();
                                if (container->inside == itemID)
                                {
                                    shopItems.emplace_back(container->inside);
                                }
                            }
                            else if (e->type->id == itemID)
                            {
                                shopItems.emplace_back(e->type->id);
                            }
                        }
                    }
                    if (shopFound)
                    {
                        Util::log(fmt::format("- FilterFindRegularShopWithSpecificContents: found shop on {}-{} at x:{} y:{} with items:", currentWorld, currentLevel, shopkeeperEntity->x,
                                              shopkeeperEntity->y));
                        for (auto shopItem : shopItems)
                        {
                            Util::log(fmt::format("\t- {} ({})", shopItem, mSeedFinder->entityName(shopItem)));
                        }

                        const auto checkShopItem = [](std::vector<uint32_t>& items, uint32_t searchItem) -> bool {
                            auto it = std::find(items.begin(), items.end(), searchItem);
                            if (it == items.end())
                            {
                                return false;
                            }
                            else
                            {
                                items.erase(it);
                                return true;
                            }
                        };

                        bool matchesRequest = true;
                        for (auto i = 0; i < 4; ++i)
                        {
                            if (mItemIDs[i] != 0)
                            {
                                if (!checkShopItem(shopItems, mItemIDs[i]))
                                {
                                    matchesRequest = false;
                                }
                            }
                        }

                        if (matchesRequest)
                        {
                            return true;
                        }
                    }
                }
            }

            return false;
        };

        bool found = false;
        auto chosenLayer = searchInLayer();
        if (*chosenLayer == LayerChoice::FRONT || *chosenLayer == LayerChoice::ALL)
        {
            if (checkForMatchingShops(state.layer(0)->items()))
            {
                found = true;
            }
        }
        if (!found && (*chosenLayer == LayerChoice::BACK || *chosenLayer == LayerChoice::ALL))
        {
            if (checkForMatchingShops(state.layer(1)->items()))
            {
                found = true;
            }
        }
        return found;
    }

    void FilterFindShopWithSpecificContents::render()
    {
        const auto renderItemCombo = [&](uint8_t index, const char** selectedValueHolder, uint32_t* valueHolder) {
            ImGui::PushItemWidth(120);
            if (ImGui::BeginCombo(fmt::format("##SeedFinderFilterFindShopWithSpecificContentsItemID{}{}", index, fmt::ptr(this)).c_str(), *selectedValueHolder))
            {
                for (auto i = 0; i < itemPossibilitiesInShop(); ++i)
                {
                    bool isSelected = (*selectedValueHolder == comboItemOption(i));
                    if (ImGui::Selectable(comboItemOption(i), isSelected))
                    {
                        *selectedValueHolder = comboItemOption(i);
                        *valueHolder = shopItemID(i);
                    }
                }
                ImGui::EndCombo();
            }
        };

        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindShopWithSpecificContentsHeader{}", filterTitle(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Items :");
            ImGui::SameLine();

            for (auto i = 0; i < itemPositionsInShop(); ++i)
            {
                renderItemCombo(i, &mComboChosenItemIDs[i], &mItemIDs[i]);
                ImGui::SameLine();
            }

            if (showLayerCombo())
            {
                ImGui::SameLine();
                FilterUI::renderLayerSelector(searchInLayer(), this);
            }
            FilterUI::renderDeleteFilterButton(this);
            FilterUI::renderLevelSelectorCheckboxes(&mLevelsToSearch);
        }
    }

    void FilterFindShopWithSpecificContents::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindShopWithSpecificContents ({})", filterTitle()));
        for (auto i = 0; i < itemPositionsInShop(); ++i)
        {
            if (mItemIDs[i] != 0)
            {
                Util::log(fmt::format("\tItem ID: {} ({})", mItemIDs[i], mSeedFinder->entityName(mItemIDs[i])));
            }
        }
        Util::log(fmt::format("\tLevel(s): {}", Util::joinVectorOfStrings(mLevelsToSearch.chosenLevels(), ", ")));
    }
} // namespace SeedFinder