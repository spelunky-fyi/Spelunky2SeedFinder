#pragma once

#include "FilterFindShopWithSpecificContents.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindCavemanShopWithSpecificContents : public FilterFindShopWithSpecificContents
    {
      public:
        explicit FilterFindCavemanShopWithSpecificContents(SeedFinder* seedFinder);
        ~FilterFindCavemanShopWithSpecificContents() override = default;

        json serialize() const override;
        std::string unserialize(const json& j) override;

        static std::string title();
        static std::string uniqueIdentifier();
        static std::unique_ptr<FilterFindCavemanShopWithSpecificContents> instantiate(SeedFinder* seedFinder);

      private:
        LayerChoice mLayer = LayerChoice::BACK;

        constexpr static size_t cShopItemsCount = 38;
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