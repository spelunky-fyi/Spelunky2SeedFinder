#include "AStar/SimplifiedLevel.h"
#include "AStar/SearchNode.h"
#include "SeedFinder.h"

namespace SeedFinder
{
    std::vector<uint16_t> SimplifiedLevel::msSolidBlockIDs = {};
    std::vector<uint16_t> SimplifiedLevel::msNonSolidBlockIDs = {};
    uint16_t SimplifiedLevel::msPushBlockID = 0;

    SimplifiedLevel::SimplifiedLevel(SeedFinder* seedFinder, uint16_t chunkWidth, uint16_t chunkheight, const EntityList::EntityRange entities, bool treatPushBlockAsSolid,
                                     const std::unordered_map<uint16_t, uint8_t>& substitutions)
        : mSeedFinder(seedFinder), mChunkWidth(chunkWidth), mChunkHeight(chunkheight)
    {
        loadBlockIDs(mSeedFinder);

        auto totalWidth = width();
        auto totalHeight = LEVEL_HEIGHT;
        mLevel = std::vector<uint8_t>(totalWidth * totalHeight, ' ');
        for (Entity* entity : entities)
        {
            uint32_t entityX = (std::max)(0u, static_cast<uint32_t>(std::round(entity->x)));
            uint32_t entityY = (std::max)(0u, static_cast<uint32_t>(std::round(entity->y)));

            auto index = (entityY * totalWidth) + entityX;
            if (index < (totalWidth * totalHeight))
            {
                if (substitutions.count(entity->type->id) > 0)
                {
                    mLevel[index] = substitutions.at(entity->type->id);
                }
                else if (treatPushBlockAsSolid && entity->type->id == msPushBlockID)
                {
                    mLevel[index] = SOLID_BLOCK;
                }
                else
                {
                    if (std::find(msSolidBlockIDs.begin(), msSolidBlockIDs.end(), entity->type->id) != msSolidBlockIDs.end())
                    {
                        mLevel[index] = SOLID_BLOCK;
                    }
                }
            }
        }
    }

    uint32_t SimplifiedLevel::width() const noexcept { return (mChunkWidth * 10) + 6; }

    uint32_t SimplifiedLevel::height() const noexcept { return (mChunkHeight * 8) + 6; }

    uint8_t SimplifiedLevel::blockValue(uint16_t x, uint16_t y) const
    {
        if (x < 0 || x >= width() || y < 0 || y >= LEVEL_HEIGHT)
        {
            return SOLID_BLOCK;
        }
        return mLevel.at((y * width()) + x);
    }

    bool SimplifiedLevel::areTilesConnected(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
    {
        if (blockValue(x1, y1) == SOLID_BLOCK)
        {
            Util::log(fmt::format("Error: SimplifiedLevel::areTilesConnected requested for block {}, {} which is a solid block.", x1, y1));
            return false;
        }
        if (blockValue(x2, y2) == SOLID_BLOCK)
        {
            Util::log(fmt::format("Error: SimplifiedLevel::areTilesConnected requested for block {}, {} which is a solid block.", x2, y2));
            return false;
        }

        AStarSearch<SearchNode> astarSearch(this);

        auto startNode = SearchNode(x1, y1);
        auto endNode = SearchNode(x2, y2);

        astarSearch.setStartAndGoalStates(startNode, endNode);
        auto searchState = astarSearch.searchStep();

        while (searchState == AStarSearch<SearchNode>::SearchState::Searching)
        {
            searchState = astarSearch.searchStep();
        }

        return (searchState == AStarSearch<SearchNode>::SearchState::Succeeded);
    }

    void SimplifiedLevel::log(std::ostream& out)
    {
        constexpr uint16_t totalHeight = LEVEL_HEIGHT;
        uint16_t totalWidth = width();
        for (auto y = totalHeight - 1; y >= 0; --y)
        {
            for (auto x = 0; x < totalWidth; ++x)
            {
                auto index = (y * totalWidth) + x;
                if (mMarkedPositions.count(index) > 0)
                {
                    out << mMarkedPositions[index];
                }
                else
                {
                    out << mLevel[index];
                }
            }
            out << "\n";
            if (y <= totalHeight - height())
            {
                break;
            }
        }
    }

    void SimplifiedLevel::markPosition(uint16_t x, uint16_t y, uint8_t character) { mMarkedPositions[(y * width()) + x] = character; }

    void SimplifiedLevel::clearMarkedPositions() { mMarkedPositions.clear(); }

    void SimplifiedLevel::loadBlockIDs(SeedFinder* seedFinder)
    {
        if (msSolidBlockIDs.empty())
        {
            msPushBlockID = to_id("ENT_TYPE_ACTIVEFLOOR_PUSHBLOCK");

            // totems seem to be at coords 0,1 for some reason, so not taken into account :/
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_PLATFORM")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_PAGODA_PLATFORM")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_LADDER")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_LADDER_PLATFORM")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_VINE")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_VINE_TREE_TOP")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_GROWABLE_VINE")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_CLIMBING_POLE")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_ENTRANCE")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_EXIT")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_LAYER")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_LOCKED")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_LOCKED_PEN")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_COG")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_MOAI_STATUE")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_PLATFORM")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_SPIKES")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_SPIKES_UPSIDEDOWN")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_MUSHROOM_BASE")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_MUSHROOM_TRUNK")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_MUSHROOM_TOP")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_MUSHROOM_HAT_PLATFORM")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_PIPE")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_QUICKSAND")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_SPRING_TRAP")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_ACTIVEFLOOR_BONEBLOCK")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_ACTIVEFLOOR_POWDERKEG")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_ACTIVEFLOOR_CHAINEDPUSHBLOCK")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_ACTIVEFLOOR_FALLING_PLATFORM")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_ACTIVEFLOOR_CRUSH_TRAP")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_ACTIVEFLOOR_CRUSH_TRAP_LARGE")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_ACTIVEFLOOR_SLIDINGWALL")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_ACTIVEFLOOR_THINICE")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_ACTIVEFLOOR_ELEVATOR")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_ACTIVEFLOOR_REGENERATINGBLOCK")));
            msNonSolidBlockIDs.emplace_back(static_cast<uint16_t>(msPushBlockID));

            for (const auto& entity : seedFinder->allEntities())
            {
                if (std::find(msNonSolidBlockIDs.begin(), msNonSolidBlockIDs.end(), entity.id) == msNonSolidBlockIDs.end() &&
                    (entity.name.substr(0, 14) == "ENT_TYPE_FLOOR" || entity.name.substr(0, 20) == "ENT_TYPE_ACTIVEFLOOR"))
                {
                    msSolidBlockIDs.emplace_back(entity.id);
                }
            }
            msSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_LIQUID_STAGNANT_LAVA")));
            msSolidBlockIDs.emplace_back(static_cast<uint16_t>(to_id("ENT_TYPE_LIQUID_LAVA")));
        }
    }

} // namespace SeedFinder