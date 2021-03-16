#pragma once

#include "FilterFindShopWithSpecificContents.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindTunShopWithSpecificContents : public FilterFindShopWithSpecificContents
    {
      public:
        explicit FilterFindTunShopWithSpecificContents(SeedFinder* seedFinder);
        ~FilterFindTunShopWithSpecificContents() override = default;
        static std::string title();

      private:
        constexpr static size_t cShopItemsCount = 33;
        LayerChoice mLayer = LayerChoice::BACK;
        static size_t msShopItemIDs[cShopItemsCount];
        static const char* msComboItemOptions[cShopItemsCount];

        size_t itemPositionsInShop() const override;
        size_t itemPossibilitiesInShop() const override;
        size_t shopItemID(int8_t index) const override;
        size_t shopKeeperID() const override;
        LayerChoice* searchInLayer() override;
        bool showLayerCombo() const override;
        std::string filterTitle() const override;
        const char* comboItemOption(size_t index) const override;
    };
} // namespace SeedFinder