#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindVanHorsing : public Filter
    {
      public:
        explicit FilterFindVanHorsing(SeedFinder* seedFinder);
        ~FilterFindVanHorsing() override = default;

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
        static std::unique_ptr<FilterFindVanHorsing> instantiate(SeedFinder* seedFinder);

      private:
        AccessibilityChoice mHorsingDoorAccessibility = AccessibilityChoice::MAYBE;
        AccessibilityChoice mKeyAccessibility = AccessibilityChoice::MAYBE;

        static uint16_t msVanHorsingID;
        static uint16_t msLayerDoorID;
        static uint16_t msKeyID;
        static const char* kJSONAccessibilityDoor;
        static const char* kJSONAccessibilityKey;
    };

} // namespace SeedFinder