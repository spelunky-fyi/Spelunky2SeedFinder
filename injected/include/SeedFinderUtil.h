#pragma once

#include "entity.hpp"
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace SeedFinder
{
    static const auto gSectionMarginHor = 25;
    static const auto gThresholdChooseJungleVolcana = 3;
    static const auto gThresholdChooseTidePoolTemple = 8;
    static const auto gThresholdChooseEggplantWorld = 18;

    class Util
    {
      public:
        /**
         * @brief Combines all the string elements in a vector to one big string, separated by the delimiter
         */
        static std::string joinVectorOfStrings(const std::vector<std::string>& v, const char* delimiter);

        /**
         * @brief Add a message to the log file
         */
        static void log(const std::string& s);

        /**
         * @brief Opens the log file for writing
         */
        static void startLogging();

        /**
         * @brief Closes the log file
         */
        static void stopLogging();

        static std::ofstream msLogStream;
    };

    enum class LayerChoice
    {
        FRONT = 0,
        BACK = 1,
        ALL = 99
    };

    enum class AccessibilityChoice
    {
        MAYBE = 0,
        YES = 1,
        NO = 2
    };

    enum class PositionChoice
    {
        WHEREVER = 0,
        HIGHER = 1,
        LOWER = 2
    };

    struct LevelStorage // checkbox selection of levels
    {
        bool OneOne = false;     // 0
        bool OneTwo = false;     // 1
        bool OneThree = false;   // 2
        bool Quillback = false;  // 3
        bool TwoOne = false;     // 4
        bool TwoTwo = false;     // 5
        bool TwoThree = false;   // 6
        bool TwoFour = false;    // 7
        bool Olmec = false;      // 8
        bool FourOne = false;    // 9
        bool FourTwo = false;    // 10
        bool FourThree = false;  // 11
        bool FourFour = false;   // 12
        bool IceCaves = false;   // 13
        bool SixOne = false;     // 14
        bool SixTwo = false;     // 15
        bool SixThree = false;   // 16
        bool Tiamat = false;     // 17
        bool SevenOne = false;   // 18
        bool SevenTwo = false;   // 19
        bool SevenThree = false; // 20
        bool Hundun = false;     // 21
        bool Any = false;        // in case of radiobuttons, but the user doesn't have to choose a level

        std::unordered_set<uint8_t> hiddenLevels = {};
        std::unordered_set<uint8_t> disabledLevels = {};

        void all();
        void none();
        uint8_t deepest() const;
        void activateDepth(uint8_t depth);
        bool isValid() const;
        bool shouldExecute(uint8_t world, uint8_t level) const;
        std::vector<std::string> chosenLevels() const;

        void hideLevel(uint8_t depth);
        void disableLevel(uint8_t depth);
        bool isHidden(uint8_t depth);
        bool isDisabled(uint8_t depth);

        void hideLevel(uint8_t world, uint8_t level);
        void disableLevel(uint8_t world, uint8_t level);
        bool isHidden(uint8_t world, uint8_t level);
        bool isDisabled(uint8_t world, uint8_t level);
    };

    struct WarpInfo
    {
        uint8_t world;
        uint8_t level;
        uint8_t theme;
        WarpInfo(uint8_t w, uint8_t l, uint8_t t) : world(w), level(l), theme(t){};
    };

    struct Rect
    {
        float x;
        float y;
        float width;
        float height;

        Rect();
        Rect(float xx, float yy, float ww, float hh);
        bool containsPoint(float xx, float yy) const;
        bool isValid() const;
    };
} // namespace SeedFinder