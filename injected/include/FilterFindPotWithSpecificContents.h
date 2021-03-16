#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindPotWithSpecificContents : public Filter
    {
      public:
        explicit FilterFindPotWithSpecificContents(SeedFinder* seedFinder);
        ~FilterFindPotWithSpecificContents() override = default;

        bool isValid() override;
        bool shouldExecute(uint8_t currentWorld, uint8_t currentLevel) override;
        bool execute(uint8_t currentWorld, uint8_t currentLevel) override;
        void render() override;
        void writeToLog() override;
        uint8_t deepestLevel() const override;
        static std::string title();

      private:
        uint32_t mItemID = 0;
        LayerChoice mLayer = LayerChoice::FRONT;
        uint8_t mMinimumAmount = 1;
        LevelStorage mLevelsToSearch;
        const char* mComboChosenItemID = nullptr;

        constexpr static size_t cItemCount = 10;

        static const char* msComboItemOptions[cItemCount];
        static uint16_t msComboItemOptionItemIDs[cItemCount];
        static uint16_t msPotID;
    };

} // namespace SeedFinder