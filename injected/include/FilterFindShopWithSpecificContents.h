#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindShopWithSpecificContents : public Filter
    {
      public:
        explicit FilterFindShopWithSpecificContents(SeedFinder* seedFinder);
        ~FilterFindShopWithSpecificContents() override = default;

        bool isValid() override;
        bool shouldExecute(uint8_t currentWorld, uint8_t currentLevel) override;
        bool execute(uint8_t currentWorld, uint8_t currentLevel) override;
        void render() override;
        void writeToLog() override;
        uint8_t deepestLevel() const override;
        json serialize() const override;
        std::string unserialize(const json& j) override;

      protected:
        LevelStorage mLevelsToSearch;
        uint16_t mItemIDs[4] = {0};
        const char* mComboChosenItemIDs[4] = {nullptr, nullptr, nullptr, nullptr};

        static const char* kJSONItem1;
        static const char* kJSONItem2;
        static const char* kJSONItem3;
        static const char* kJSONItem4;
        static const char* msComboItemOptions[4];

      private:
        virtual size_t itemPositionsInShop() const = 0;
        virtual size_t itemPossibilitiesInShop() const = 0;
        virtual size_t shopKeeperID() const = 0;
        virtual size_t shopItemID(int8_t index) const = 0;
        virtual LayerChoice* searchInLayer() = 0;
        virtual bool showLayerCombo() const = 0;
        virtual std::string filterTitle() const = 0;
        virtual const char* comboItemOption(size_t index) const = 0;

        static size_t msBGShopID;
        static size_t msPresentID;
    };
} // namespace SeedFinder