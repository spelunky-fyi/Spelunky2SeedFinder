#pragma once
#include "AStar/AStar.h"
#include <cstdint>

namespace SeedFinder
{
    class SearchNode
    {
      public:
        uint8_t x;
        uint8_t y;

        SearchNode();
        SearchNode(uint8_t px, uint8_t py);

        float goalDistanceEstimate(SearchNode& nodeGoal);
        bool isGoal(SearchNode& nodeGoal);
        void getSuccessors(AStarSearch<SearchNode>* astarsearch, SearchNode* parent_node);
        float getCost(SearchNode& successor);
        bool isSameState(SearchNode& other);
    };

} // namespace SeedFinder