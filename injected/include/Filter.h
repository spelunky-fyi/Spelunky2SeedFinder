#pragma once
#include "FilterUI.h"
#include "SeedFinderUtil.h"
#include "state.hpp"
#include <cstdint>
#include <imgui.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace SeedFinder
{
    class SeedFinder;

    class Filter
    {
      public:
        explicit Filter(SeedFinder* seedFinder);
        virtual ~Filter() = default;

        /**
         * @brief Checks whether the filter is valid, e.g. whether all fields are filled in correctly
         */
        virtual bool isValid() = 0;

        /**
         * @brief Checks whether the filter should execute on the provided world and level combination
         */
        virtual bool shouldExecute(uint8_t currentWorld, uint8_t currentLevel) = 0;

        /**
         * @brief Executes the filter: get the state/layers and do something
         * @return True if the filter matches, false if not
         */
        virtual bool execute(uint8_t currentWorld, uint8_t currentLevel) = 0;

        /**
         * @brief Renders the filter configuration
         */
        virtual void render() = 0;

        /**
         * @brief Write out filter information to the log file
         */
        virtual void writeToLog() = 0;

        /**
         * @brief Serialize the filter to json
         */
        virtual json serialize() const = 0;

        /**
         * @brief Unserialize the provided json-data
         * @return An error message, or an empty string in case of no errors
         */
        virtual std::string unserialize(const json& j) = 0;

        /**
         * @brief Calculate the deepest level to which this filter applies. See the numbers in the comments of the LevelStorage struct, or just pass
         * LevelStorage::deepest()
         */
        virtual uint8_t deepestLevel() const = 0;

        /**
         * @brief Will be called when a new seed is being processed, so the filter can reset all its internal values.
         * This is handy if you need to keep track of stuff throughout multiple levels, e.g. have we seen the udjat eye, drill, ...
         * In other words, things that can happen on multiple levels
         */
        virtual void resetForNewSeed(uint32_t newSeed){};

        /**
         * @brief Sets the last error that occurred while validating the filter, so a proper message can be shown to the user about the misconfiguration
         * of the filter
         */
        void setLastError(const std::string& error);
        std::string lastError() const;

        void markAsDeleted() noexcept
        {
            mDeleted = true;
        }

        bool isDeleted() const noexcept
        {
            return mDeleted;
        }

        static uint16_t msStartDoorID;

      protected:
        SeedFinder* mSeedFinder = nullptr;

      private:
        bool mDeleted = false;
        std::string mLastError;
    };

} // namespace SeedFinder