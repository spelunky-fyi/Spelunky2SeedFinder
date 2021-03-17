#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindSisters : public Filter
    {
      public:
        explicit FilterFindSisters(SeedFinder* seedFinder);
        ~FilterFindSisters() override = default;

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
        static std::unique_ptr<FilterFindSisters> instantiate(SeedFinder* seedFinder);

      private:
        AccessibilityChoice mAccessibility = AccessibilityChoice::MAYBE;
        LevelStorage mLevelsToSearch;

        static uint16_t msSisterParsleyID;
        static uint16_t msSisterParsnipID;
        static uint16_t msSisterParmesanID;
        static uint16_t msLayerDoorID;
    };
} // namespace SeedFinder