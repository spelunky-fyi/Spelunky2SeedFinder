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
        json serialize() const override;
        std::string unserialize(const json& j) override;

        static std::string title();
        static std::string uniqueIdentifier();
        static std::unique_ptr<FilterFindQuillbackObstruction> instantiate(SeedFinder* seedFinder);

      private:
        bool mCheckJungleSide;
        bool mCheckVolcanaSide;
        const char* mExistance = msExistances[0];

        static uint16_t msExitDoorID;
        static uint16_t msLayerDoorID;
        static const char* msSides[];
        static const char* msExistances[];
        static const char* kJSONCheckJungleSide;
        static const char* kJSONCheckVolcanaSide;
        static const char* kJSONExistance;
    };

} // namespace SeedFinder