#pragma once

#include "Filter.h"

namespace SeedFinder
{
    class SeedFinder;
    class FilterFindAnkhSkip : public Filter
    {
      public:
        explicit FilterFindAnkhSkip(SeedFinder* seedFinder);
        ~FilterFindAnkhSkip() override = default;

        bool isValid() override;
        bool shouldExecute(uint8_t currentWorld, uint8_t currentLevel) override;
        bool execute(uint8_t currentWorld, uint8_t currentLevel) override;
        void render() override;
        void writeToLog() override;
        uint8_t deepestLevel() const override;
        static std::string title();

      private:
        bool mCheckSidePagoda = true;
        bool mCheckLayerDoor = false;

        static uint16_t msLayerDoorID;
        static uint16_t msPagodaID;
    };

} // namespace SeedFinder