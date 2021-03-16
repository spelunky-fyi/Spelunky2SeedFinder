#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindLevelFeelings : public Filter
    {
      public:
        explicit FilterFindLevelFeelings(SeedFinder* seedFinder);
        ~FilterFindLevelFeelings() override = default;

        bool isValid() override;
        bool shouldExecute(uint8_t currentWorld, uint8_t currentLevel) override;
        bool execute(uint8_t currentWorld, uint8_t currentLevel) override;
        void render() override;
        void writeToLog() override;
        uint8_t deepestLevel() const override;
        static std::string title();

      private:
        LevelStorage mLevelsToSearch;
        const char* mComboChosenFeeling = nullptr;
        const char* mComboChosenNecessity = msComboItemNecessities[0];

        constexpr static size_t cFeelingsCount = 7;
        static const char* msComboItemOptions[];
        static const char* msComboItemNecessities[];
    };
} // namespace SeedFinder