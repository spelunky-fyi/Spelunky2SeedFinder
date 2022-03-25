#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindSunChallenge : public Filter
    {
      public:
        explicit FilterFindSunChallenge(SeedFinder* seedFinder);
        ~FilterFindSunChallenge() override = default;

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
        static std::unique_ptr<FilterFindSunChallenge> instantiate(SeedFinder* seedFinder);

      private:
        LevelStorage mLevelsToSearch;
        AccessibilityChoice mAccessibility = AccessibilityChoice::MAYBE;

        uint8_t mCurrentWorld;
        uint8_t mCurrentLevel;
        EntityList::EntityRange mFrontLayerEntities;
        bool mChallengeFound = false;
        uint8_t mChallengeFoundOnLevel = 0;
        uint8_t mTunX = 0;
        uint8_t mTunY = 0;

        static uint16_t msTunID;

        void locateTun();
    };

} // namespace SeedFinder