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
        static std::string title();
        void resetForNewSeed(uint32_t newSeed) override;

      private:
        LevelStorage mLevelsToSearch;
        uint8_t mCurrentWorld;
        uint8_t mCurrentLevel;
        bool mDrillFound = false;
        uint8_t mDrillFoundOnLevel = 0;
        const char* mChosenComparison = msComparisonOptions[0];
        uint16_t mAmountOfLavaBlocks = 100;
        AccessibilityChoice mShopUnderDrill = AccessibilityChoice::MAYBE;
        AccessibilityChoice mAltarUnderDrill = AccessibilityChoice::MAYBE;

        std::vector<Entity*> mFrontLayerEntities;
        uint8_t mDrillX = 0;

        static uint16_t msDrillIndicatorID;
        static uint16_t msStagnantLavaID;
        static uint16_t msLiquidLavaID;
        static uint16_t msAltarID;
        static uint16_t msShopBGID;
        static const char* msComparisonOptions[];

        void locateDrill();
    };

} // namespace SeedFinder