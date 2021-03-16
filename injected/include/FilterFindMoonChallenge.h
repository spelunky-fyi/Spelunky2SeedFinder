#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindMoonChallenge : public Filter
    {
      public:
        explicit FilterFindMoonChallenge(SeedFinder* seedFinder);
        ~FilterFindMoonChallenge() override = default;

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
        bool mChallengeFound = false;
        uint8_t mChallengeFoundOnLevel = 0;
        AccessibilityChoice mAccessibility = AccessibilityChoice::MAYBE;
        uint8_t mMinimumMattockDurability = 39;
        const char* mChosenMattockComparison = msComparisonOptions[1];

        std::vector<Entity*> mFrontLayerEntities;
        uint8_t mTunX = 0;
        uint8_t mTunY = 0;

        static uint16_t msTunID;
        static uint16_t msMattockID;
        static const char* msComparisonOptions[];

        void locateTun();
    };

} // namespace SeedFinder