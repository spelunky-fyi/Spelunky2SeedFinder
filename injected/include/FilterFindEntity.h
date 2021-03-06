#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindEntity : public Filter
    {
      public:
        explicit FilterFindEntity(SeedFinder* seedFinder);
        ~FilterFindEntity() override = default;

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
        static std::unique_ptr<FilterFindEntity> instantiate(SeedFinder* seedFinder);

      private:
        uint32_t mItemID = 0;
        LayerChoice mLayer = LayerChoice::FRONT;
        uint8_t mMinimumAmount = 1;
        LevelStorage mLevelsToSearch;

        const char* mComboChosenItem = nullptr;
    };
} // namespace SeedFinder