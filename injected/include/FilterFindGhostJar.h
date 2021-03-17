#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindGhostJar : public Filter
    {
      public:
        explicit FilterFindGhostJar(SeedFinder* seedFinder);
        ~FilterFindGhostJar() override = default;

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
        static std::unique_ptr<FilterFindGhostJar> instantiate(SeedFinder* seedFinder);

      private:
        AccessibilityChoice mAccessibility = AccessibilityChoice::MAYBE;
        LevelStorage mLevelsToSearch;

        static uint16_t msGhostJarID;
        static uint16_t msLayerDoorID;
        static uint16_t msLayerLockedDoorID;
        static uint16_t msLayerLockedDoorPenID;
    };
} // namespace SeedFinder