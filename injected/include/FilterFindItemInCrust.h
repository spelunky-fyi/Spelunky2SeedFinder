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
        json serialize() const override;
        std::string unserialize(const json& j) override;

        static std::string title();
        static std::string uniqueIdentifier();
        static std::unique_ptr<FilterFindItemInCrust> instantiate(SeedFinder* seedFinder);

      private:
        uint16_t mItemID = 0;
        LayerChoice mLayer = LayerChoice::FRONT;
        LevelStorage mLevelsToSearch;

        const char* mComboChosenItemID = nullptr;
        constexpr static size_t cCrustItemsCount = 30;
        static size_t msCrustItemIDs[cCrustItemsCount];
        static const char* msComboItemOptions[cCrustItemsCount];
    };

} // namespace SeedFinder