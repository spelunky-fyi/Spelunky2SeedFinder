#pragma once

#include "Filter.h"
#include "SeedFinderUtil.h"
#include "entity.hpp"
#include <chrono>
#include <cstdint>
#include <memory>
#include <vector>

// #define SEEDFINDER_WARPTEST

namespace SeedFinder
{
    class SeedFinder
    {
      public:
        SeedFinder();
        virtual ~SeedFinder() = default;

        /**
         * @brief Renders the SeedFinder UI
         */
        void render();

        /**
         * @brief Makes a copy of all entities
         */
        void registerAllEntities(const std::vector<EntityItem>& entities);

        /**
         * @brief Registers all filter titles that are available
         */
        void registerFilterTitles();

        /**
         * @brief Add a filter that is to be performed when searching for seeds
         */
        void addFilterByTitle(const char* filterTitle);

        /**
         * @brief Retrieves the name of an entity by its ID
         */
        const char* entityName(uint16_t id);

        /**
         * @brief Retrieves the vector containing all registered entities
         */
        const std::vector<EntityItem>& allEntities();

        /**
         * @brief Looks up all the entities at a specific x,y coordinate
         */
        std::vector<Entity*> entitiesInRect(const Rect& rect, const std::vector<Entity*>& items);

        uint32_t currentSeed() const noexcept;

        bool areWeGoingToVolcana();
        bool areWeGoingToJungle();
        bool areWeGoingToTemple();
        bool areWeGoingToTidePool();

      private:
        enum class SeedFinderState
        {
            IDLE,
            PAUSE,
            INIT,
            EXECUTE_LOAD_SEED,
            EXECUTE_FILTERS,
            CLEANUP
        };

        // configuration state
        uint32_t mAmountToProcess = 10;
        uint32_t msStartSeed = 0;
        bool mUseRandomSeeds = true;
        bool mGoToJungle = true;
        bool mGoToTidePool = true;
        bool mGoToCityOfGold = false;
        bool mGoToAbzu = false;
        bool mGoToDuat = false;
        bool mGoToEggplantWorld = false;
        uint32_t mAntiFlickerFrameCount = 30;

        // seed finder state
        SeedFinderState mSeedFinderState = SeedFinderState::IDLE;
        SeedFinderState mSeedFinderNextState = SeedFinderState::IDLE;
        uint32_t mSeedFinderPauseCounter = 0;
        uint8_t mDeepestLevel = 0;
        uint32_t mCurrentSeed = 0;
        uint32_t mAmountOfSeedsProcessed = 0;
        std::vector<uint32_t> mFoundSeeds;
        std::chrono::steady_clock::time_point mTimeStart;
        bool mNextLevelIsEggplantWorld;

        // filters state
        std::vector<std::unique_ptr<Filter>> mFilters = {};
        std::vector<std::string> mFilterTitles = {};
        std::vector<EntityItem> mAllEntities = {};
        const char* mComboAddFilterChosenTitle = nullptr;

        void handleState();
        void initializeNewRun();
        void executeLoadSeed();
        void executeFilters();
        void cleanUp();
        void pause(uint16_t frameCount, SeedFinderState nextState);

        uint8_t deepestLevelOfAllFilters();
        bool areAllFiltersValid();
        WarpInfo warpInfoForDepth(uint8_t d);
        void checkLevelContainsMotherStatue();
        uint32_t nextSeed(uint32_t currentSeed);
        void setGameSeed(uint32_t seed);
        void warp(uint8_t world, uint8_t level, uint8_t theme);

#ifdef SEEDFINDER_WARPTEST
        std::string calculateLevelSHA256(uint32_t seed, uint8_t world, uint8_t level, uint8_t theme);
        void warpTest();
#endif

        static uint16_t mEggplantMotherStatueHandID;

        static const uint8_t THEME_DWELLING = 1;
        static const uint8_t THEME_JUNGLE = 2;
        static const uint8_t THEME_VOLCANA = 3;
        static const uint8_t THEME_OLMEC = 4;
        static const uint8_t THEME_TIDE_POOL = 5;
        static const uint8_t THEME_TEMPLE = 6;
        static const uint8_t THEME_ICE_CAVES = 7;
        static const uint8_t THEME_NEO_BABYLON = 8;
        static const uint8_t THEME_SUNKEN_CITY = 9;
        static const uint8_t THEME_COSMIC_OCEAN = 10;
        static const uint8_t THEME_CITY_OF_GOLD = 11;
        static const uint8_t THEME_DUAT = 12;
        static const uint8_t THEME_ABZU = 13;
        static const uint8_t THEME_TIAMAT = 14;
        static const uint8_t THEME_EGGPLANT_WORLD = 15;
        static const uint8_t THEME_HUNDUN = 16;
    };
} // namespace SeedFinder