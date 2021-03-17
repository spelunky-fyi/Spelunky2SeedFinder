#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindCrateWithSpecificContents : public Filter
    {
      public:
        explicit FilterFindCrateWithSpecificContents(SeedFinder* seedFinder);
        ~FilterFindCrateWithSpecificContents() override = default;

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
        static std::unique_ptr<FilterFindCrateWithSpecificContents> instantiate(SeedFinder* seedFinder);

      private:
        uint16_t mItemID = 0;
        LayerChoice mLayer = LayerChoice::FRONT;
        uint8_t mMinimumAmount = 1;
        LevelStorage mLevelsToSearch;

        constexpr static size_t cItemCount = 24;
        const char* mComboChosenItemID = nullptr;
        static const char* msComboItemOptions[cItemCount];
        static uint16_t msComboItemOptionItemIDs[cItemCount];
        static uint16_t msCrateID;
    };

} // namespace SeedFinder