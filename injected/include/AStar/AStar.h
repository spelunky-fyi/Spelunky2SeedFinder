#pragma once
#include "SimplifiedLevel.h"
#include <algorithm>
#include <memory>
#include <vector>

// based on https://github.com/justinhj/astar-algorithm-cpp

namespace SeedFinder
{

    template <class UserState> class AStarSearch
    {
      public:
        enum class SearchState
        {
            NotInitialised,
            Searching,
            Succeeded,
            Failed,
            Invalid
        };

        class Node
        {
          public:
            Node* parent;
            Node* child;
            float g, h, f;
            Node() : parent(nullptr), child(nullptr), g(0.0f), h(0.0f), f(0.0f){};

            UserState mUserState;
        };

        class HeapCompareF
        {
          public:
            bool operator()(const Node* x, const Node* y) const { return x->f > y->f; }
        };

      public:
        AStarSearch(SimplifiedLevel* level) : mState(SearchState::NotInitialised), mCurrentSolutionNode(nullptr), mLevel(level) {}

        SimplifiedLevel* level() const noexcept { return mLevel; }

        void setStartAndGoalStates(UserState& start, UserState& goal)
        {
            mStartNode = allocateNode();
            mGoalNode = allocateNode();
            mStartNode->mUserState = start;
            mGoalNode->mUserState = goal;

            mState = SearchState::Searching;

            mStartNode->g = 0;
            mStartNode->h = mStartNode->mUserState.goalDistanceEstimate(mGoalNode->mUserState);
            mStartNode->f = mStartNode->g + mStartNode->h;
            mStartNode->parent = nullptr;
            mOpenList.emplace_back(mStartNode);
            std::push_heap(mOpenList.begin(), mOpenList.end(), HeapCompareF());
            mSteps = 0;
        }

        SearchState searchStep()
        {
            if (mState == SearchState::Succeeded || mState == SearchState ::Failed)
            {
                return mState;
            }
            if (mOpenList.empty())
            {
                mState = SearchState::Failed;
                return mState;
            }
            mSteps++;

            Node* n = mOpenList.front();
            std::pop_heap(mOpenList.begin(), mOpenList.end(), HeapCompareF());
            mOpenList.pop_back();

            if (n->mUserState.isGoal(mGoalNode->mUserState))
            {
                mGoalNode->parent = n->parent;
                mGoalNode->g = n->g;

                if (!n->mUserState.isSameState(mStartNode->mUserState))
                {
                    Node* nodeChild = mGoalNode;
                    Node* nodeParent = mGoalNode->parent;
                    do
                    {
                        nodeParent->child = nodeChild;
                        nodeChild = nodeParent;
                        nodeParent = nodeParent->parent;
                    } while (nodeChild != mStartNode);
                }
                mState = SearchState::Succeeded;
                return mState;
            }
            else
            {
                mSuccessors.clear();
                n->mUserState.getSuccessors(this, n->parent ? &n->parent->mUserState : nullptr);
                for (Node* successor : mSuccessors)
                {
                    float newg = n->g + n->mUserState.getCost(successor->mUserState);

                    Node* foundInOpenList = nullptr;
                    for (Node* openListResult : mOpenList)
                    {
                        if (openListResult->mUserState.isSameState(successor->mUserState))
                        {
                            foundInOpenList = openListResult;
                            break;
                        }
                    }
                    if (foundInOpenList != nullptr && foundInOpenList->g <= newg)
                    {
                        continue;
                    }

                    Node* foundInClosedList = nullptr;
                    for (Node* closedListResult : mClosedList)
                    {
                        if (closedListResult->mUserState.isSameState(successor->mUserState))
                        {
                            foundInClosedList = closedListResult;
                            break;
                        }
                    }
                    if (foundInClosedList != nullptr && foundInClosedList->g <= newg)
                    {
                        continue;
                    }

                    successor->parent = n;
                    successor->g = newg;
                    successor->h = successor->mUserState.goalDistanceEstimate(mGoalNode->mUserState);
                    successor->f = successor->g + successor->h;

                    if (foundInClosedList != nullptr)
                    {
                        foundInClosedList->parent = successor->parent;
                        foundInClosedList->g = successor->g;
                        foundInClosedList->h = successor->h;
                        foundInClosedList->f = successor->f;
                        mOpenList.emplace_back(foundInClosedList);
                        mClosedList.erase(std::find(mClosedList.begin(), mClosedList.end(), foundInClosedList), mClosedList.end());
                        std::push_heap(mOpenList.begin(), mOpenList.end(), HeapCompareF());
                    }
                    else if (foundInOpenList != nullptr)
                    {
                        foundInOpenList->parent = successor->parent;
                        foundInOpenList->g = successor->g;
                        foundInOpenList->h = successor->h;
                        foundInOpenList->f = successor->f;
                        std::make_heap(mOpenList.begin(), mOpenList.end(), HeapCompareF());
                    }
                    else
                    {
                        mOpenList.emplace_back(successor);
                        std::push_heap(mOpenList.begin(), mOpenList.end(), HeapCompareF());
                    }
                }
                mClosedList.emplace_back(n);
            }
            return mState;
        }

        void addSuccessor(UserState state)
        {
            auto node = allocateNode();
            node->mUserState = std::move(state);
            mSuccessors.emplace_back(node);
        }

        uint32_t getStepCount() { return mSteps; }

      private:
        std::vector<Node*> mOpenList;
        std::vector<Node*> mClosedList;
        std::vector<Node*> mSuccessors;
        SearchState mState;
        uint32_t mSteps;
        Node* mStartNode;
        Node* mGoalNode;
        Node* mCurrentSolutionNode;
        std::vector<std::unique_ptr<Node>> mNodeStorage;
        SimplifiedLevel* mLevel;

        Node* allocateNode()
        {
            auto node = std::make_unique<Node>();
            auto nodePtr = node.get();
            mNodeStorage.push_back(std::move(node));
            return nodePtr;
        }
    };

    template <class T> class AStarState
    {
      public:
        virtual ~AStarState() = default;
        virtual float goalDistanceEstimate(T& nodeGoal) = 0;
        virtual bool isGoal(T& nodeGoal) = 0;
        virtual void getSuccessors(AStarSearch<T>* aStarSearch, T* parentNode) = 0;
        virtual float getCost(T& successor) = 0;
        virtual bool isSameState(T& other) = 0;
    };

} // namespace SeedFinder