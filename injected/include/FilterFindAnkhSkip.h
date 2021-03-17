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
        json serialize() const override;
        std::string unserialize(const json& j) override;

        static std::string title();
        static std::string uniqueIdentifier();
        static std::unique_ptr<FilterFindAnkhSkip> instantiate(SeedFinder* seedFinder);

      private:
        bool mCheckSidePagoda = true;
        bool mCheckLayerDoor = false;

        static uint16_t msLayerDoorID;
        static uint16_t msPagodaID;
        static const char* kJSONCheckLayerDoor;
        static const char* kJSONCheckSidePagoda;
    };

} // namespace SeedFinder