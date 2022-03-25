#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindVolcanaDrill : public Filter
    {
      public:
        explicit FilterFindVolcanaDrill(SeedFinder* seedFinder);
        ~FilterFindVolcanaDrill() override = default;

        bool isValid() override;
        bool shouldExecute(uint8_t currentWorld, uint8_t currentLevel) override;
        bool execute(uint8_t currentWorld, uint8_t currentLevel) override;
        void render() override;
        void writeToLog() override;
        uint8_t deepestLevel() const override;
        void resetForNewSeed(uint32_t newSeed) override;
        json serialize() const override;
        std::string unserialize(const json& j) override;

        static std::string title();
        static std::string uniqueIdentifier();
        static std::unique_ptr<FilterFindVolcanaDrill> instantiate(SeedFinder* seedFinder);

      private:
        LevelStorage mLevelsToSearch;
        uint16_t mAmountOfLavaBlocks = 100;
        const char* mChosenComparison = msComparisonOptions[0];
        AccessibilityChoice mShopUnderDrill = AccessibilityChoice::MAYBE;
        AccessibilityChoice mAltarUnderDrill = AccessibilityChoice::MAYBE;

        uint8_t mCurrentWorld;
        uint8_t mCurrentLevel;
        EntityList::EntityRange mFrontLayerEntities;
        bool mDrillFound = false;
        uint8_t mDrillFoundOnLevel = 0;
        uint8_t mDrillX = 0;

        static uint16_t msDrillIndicatorID;
        static uint16_t msStagnantLavaID;
        static uint16_t msLiquidLavaID;
        static uint16_t msAltarID;
        static uint16_t msShopBGID;
        static const char* msComparisonOptions[];
        static const char* kJSONAmountOfLavaBlocks;
        static const char* kJSONComparison;
        static const char* kJSONShopUnderDrill;
        static const char* kJSONAltarUnderDrill;

        void locateDrill();
    };

} // namespace SeedFinder