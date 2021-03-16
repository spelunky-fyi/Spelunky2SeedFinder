#include "AStar/SearchNode.h"
#include "AStar/SimplifiedLevel.h"
#include <iostream>

namespace SeedFinder
{

    SearchNode::SearchNode() : x(0), y(0) {}

    SearchNode::SearchNode(uint8_t px, uint8_t py) : x(px), y(py) {}

    float SearchNode::goalDistanceEstimate(SearchNode& nodeGoal) { return std::abs(x - nodeGoal.x) + std::abs(y - nodeGoal.y); }

    bool SearchNode::isSameState(SearchNode& other) { return (x == other.x && y == other.y); }

    bool SearchNode::isGoal(SearchNode& nodeGoal) { return (x == nodeGoal.x && y == nodeGoal.y); }

    void SearchNode::getSuccessors(AStarSearch<SearchNode>* astarsearch, SearchNode* parent_node)
    {
        uint8_t parentX = -1;
        uint8_t parentY = -1;

        if (parent_node != nullptr)
        {
            parentX = parent_node->x;
            parentY = parent_node->y;
        }

        const auto add = [&](uint8_t xx, uint8_t yy) -> void {
            if (astarsearch->level()->blockValue(xx, yy) != SimplifiedLevel::SOLID_BLOCK && !(parentX == xx && parentY == yy))
            {
                astarsearch->addSuccessor(SearchNode(xx, yy));
            }
        };

        // left
        add(x - 1, y);
        // top
        add(x, y - 1);
        // right
        add(x + 1, y);
        // bottom
        add(x, y + 1);
    }

    float SearchNode::getCost(SearchNode& successor) { return 1.0f; }

} // namespace SeedFinder