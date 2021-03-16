#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindQuillbackObstruction : public Filter
    {
      public:
        explicit FilterFindQuillbackObstruction(SeedFinder* seedFinder);
        ~FilterFindQuillbackObstruction() override = default;

        bool isValid() override;
        bool shouldExecute(uint8_t currentWorld, uint8_t currentLevel) override;
        bool execute(uint8_t currentWorld, uint8_t currentLevel) override;
        void render() override;
        void writeToLog() override;
        uint8_t deepestLevel() const override;
        static std::string title();

      private:
        bool mCheckJungleSide;
        bool mCheckVolcanaSide;
        const char* mExistance;

        static uint16_t msExitDoorID;
        static uint16_t msLayerDoorID;
        static const char* msSides[];
        static const char* msExistances[];
    };

} // namespace SeedFinder