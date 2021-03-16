#pragma once

#include "SeedFinderUtil.h"
#include <imgui.h>

namespace SeedFinder
{
    class Filter;
    class FilterUI
    {
      public:
        /**
         * @brief Draws a level selector as a set of checkboxes
         */
        static void renderLevelSelectorCheckboxes(LevelStorage* levelsToSearch, bool showAllNoneButtons = true);

        /**
         * @brief Draws a level selector as a set of radiobuttons
         */
        static void renderLevelSelectorRadiobuttons(LevelStorage* levelsToSearch, bool showAnyButton = false);

        /**
         * @brief Draws a layer selector (all/front/back)
         */
        static void renderLayerSelector(LayerChoice* layer, Filter* filter);

        /**
         * @brief Draws the x-button that removes the chosen filter from the list
         */
        static void renderDeleteFilterButton(Filter* filter);

        /**
         * @brief Draws a choice to select wether the item should or shouldn't be accessible
         */
        static void renderAccessibilityChoice(AccessibilityChoice* accessibility, Filter* filter, const char* prefix, const char* suffix);

        /**
         * @brief Draws a choice to select wether the item is higher, lower or wherever than something else
         */
        static void renderPositionChoice(PositionChoice* position, Filter* filter, const char* prefix, const char* suffix);
    };
} // namespace SeedFinder