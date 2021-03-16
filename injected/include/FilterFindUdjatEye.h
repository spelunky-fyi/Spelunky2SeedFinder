#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindUdjatEye : public Filter
    {
      public:
        explicit FilterFindUdjatEye(SeedFinder* seedFinder);
        ~FilterFindUdjatEye() override = default;

        bool isValid() override;
        bool shouldExecute(uint8_t currentWorld, uint8_t currentLevel) override;
        bool execute(uint8_t currentWorld, uint8_t currentLevel) override;
        void render() override;
        void writeToLog() override;
        uint8_t deepestLevel() const override;
        static std::string title();
        void resetForNewSeed(uint32_t newSeed) override;

      private:
        AccessibilityChoice mKeyAccessibility = AccessibilityChoice::MAYBE;
        AccessibilityChoice mDoorAccessibility = AccessibilityChoice::MAYBE;
        PositionChoice mKeyPosition = PositionChoice::WHEREVER;
        LevelStorage mLevelsToSearch;

        uint8_t mStartDoorX = 0;
        uint8_t mStartDoorY = 0;
        uint8_t mUdjatDoorX = 0;
        uint8_t mUdjatDoorY = 0;
        uint8_t mUdjatKeyX = 0;
        uint8_t mUdjatKeyY = 0;
        uint8_t mCurrentWorld;
        uint8_t mCurrentLevel;

        bool mUdjatKeyFound = false;
        uint8_t mUdjatKeyFoundOnLevel = 0;
        std::vector<Entity*> mFrontLayerEntities;
        std::vector<Entity*> mBackLayerEntities;

        static uint16_t msUdjatKeyID;
        static uint16_t msUdjatChestID;
        static uint16_t msLayerDoorID;

        void locateUdjatKeyAndStartDoor();
        void locateUdjatDoor();
    };

} // namespace SeedFinder