#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindItemInCrust : public Filter
    {
      public:
        explicit FilterFindItemInCrust(SeedFinder* seedFinder);
        ~FilterFindItemInCrust() override = default;

        bool isValid() override;
        bool shouldExecute(uint8_t currentWorld, uint8_t currentLevel) override;
        bool execute(uint8_t currentWorld, uint8_t currentLevel) override;
        void render() override;
        void writeToLog() override;
        uint8_t deepestLevel() const override;
        static std::string title();

      private:
        uint16_t mItem = 0;
        const char* mComboChosenItemID = nullptr;
        constexpr static size_t cCrustItemsCount = 30;
        LayerChoice mLayer = LayerChoice::FRONT;
        static size_t msCrustItemIDs[cCrustItemsCount];
        static const char* msComboItemOptions[cCrustItemsCount];

        LevelStorage mLevelsToSearch;
    };

} // namespace SeedFinder