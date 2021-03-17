#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindBlackMarket : public Filter
    {
      public:
        explicit FilterFindBlackMarket(SeedFinder* seedFinder);
        ~FilterFindBlackMarket() override = default;

        bool isValid() override;
        bool shouldExecute(uint8_t currentWorld, uint8_t currentLevel) override;
        bool execute(uint8_t currentWorld, uint8_t currentLevel) override;
        void render() override;
        void writeToLog() override;
        uint8_t deepestLevel() const override;
        json serialize() const override;
        void resetForNewSeed(uint32_t newSeed) override;
        std::string unserialize(const json& j) override;

        static std::string title();
        static std::string uniqueIdentifier();
        static std::unique_ptr<FilterFindBlackMarket> instantiate(SeedFinder* seedFinder);

      private:
        LevelStorage mLevelsToSearch;

        uint8_t mCurrentWorld;
        uint8_t mCurrentLevel;
        bool mBlackMarketFound = false;
        uint8_t mBlackMarketFoundOnLevel = 0;
        std::vector<Entity*> mFrontLayerEntities;

        static uint16_t msHedjetID;

        void locateBlackMarket();
    };
} // namespace SeedFinder