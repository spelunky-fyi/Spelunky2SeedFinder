#pragma once

#include "entity.hpp"
#include <vector>

namespace SeedFinder
{
    class SeedFinder;
    class SimplifiedLevel
    {
      public:
        /**
         * @brief Constructs a simplified level, converting the entities into simple characters in an array
         * @param chunkWidth The amount of horizontal chunks/rooms, as found in the gamestate
         * @param chunkHeight The amount of vertical chunks/rooms, as found in the gamestate
         * @param entities The entities that the level is made up of
         * @param treatPushBlockAsSolid Some tests need to see pushblocks as solid instead of (the default) non-solid. Default is non-solid because most
         * pushblocks can be moved, clearing the path
         * @param subtitutions A map providing entityID -> character mapping, stating what entities need to have a custom character in the simplified
         * level
         *
         */
        SimplifiedLevel(SeedFinder* seedFinder, uint16_t chunkWidth, uint16_t chunkheight, const std::vector<Entity*> entities, bool treatPushBlockAsSolid = false,
                        const std::unordered_map<uint16_t, uint8_t>& substitutions = {});
        virtual ~SimplifiedLevel() = default;

        /**
         * @brief Determines whether two tiles (x1, y1) and (x2, y2) are connected to each other without having to use a bomb
         */
        bool areTilesConnected(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

        /**
         * @brief Writes the level to the provided output stream
         */
        void log(std::ostream& out);

        /**
         * @brief Returns the horizontal amount of blocks in the level
         */
        uint32_t width() const noexcept;

        /**
         * @brief Returns the vertical amount of blocks in the level
         */
        uint32_t height() const noexcept;

        /**
         * @brief Returns either SOLID_BLOCK or NON_SOLID_BLOCK depending on the provided coordinate
         */
        uint8_t blockValue(uint16_t x, uint16_t y) const;

        /**
         * @brief Marks a certain x,y position on the level with a specific character
         */
        void markPosition(uint16_t x, uint16_t y, uint8_t character);

        /**
         * @brief Removes all marked positions
         */
        void clearMarkedPositions();

        static const uint8_t SOLID_BLOCK = 'X';
        static const uint8_t NON_SOLID_BLOCK = ' ';
        static const uint8_t LEVEL_HEIGHT = 126;

      private:
        SeedFinder* mSeedFinder;
        uint16_t mChunkWidth;
        uint16_t mChunkHeight;
        std::vector<uint8_t> mLevel;
        std::unordered_map<uint32_t, uint8_t> mMarkedPositions;

        static void loadBlockIDs(SeedFinder* seedFinder);
        static uint16_t msPushBlockID;
        static std::vector<uint16_t> msSolidBlockIDs;
        static std::vector<uint16_t> msNonSolidBlockIDs;
    };
} // namespace SeedFinder