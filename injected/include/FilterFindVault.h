#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindVault : public Filter
    {
      public:
        explicit FilterFindVault(SeedFinder* seedFinder);
        ~FilterFindVault() override = default;

        bool isValid() override;
        bool shouldExecute(uint8_t currentWorld, uint8_t currentLevel) override;
        bool execute(uint8_t currentWorld, uint8_t currentLevel) override;
        void render() override;
        void writeToLog() override;
        uint8_t deepestLevel() const override;
        json serialize() const override;
        std::string unserialize(const json& j) override;

        static std::string title();
        static std::string uniqueIdentifier();
        static std::unique_ptr<FilterFindVault> instantiate(SeedFinder* seedFinder);

      private:
        LevelStorage mLevelsToSearch;

        static uint16_t msVaultChestID;
    };
} // namespace SeedFinder