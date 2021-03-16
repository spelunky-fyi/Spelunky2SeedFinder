#include "Filter.h"
#include "SeedFinder.h"

namespace SeedFinder
{

    uint16_t Filter::msStartDoorID = 0;

    Filter::Filter(SeedFinder* seedFinder) : mSeedFinder(seedFinder)
    {
        if (msStartDoorID == 0)
        {
            msStartDoorID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_ENTRANCE"));
        }
    }

    void Filter::setLastError(const std::string& error) { mLastError = error; }

    std::string Filter::lastError() const { return mLastError; }
} // namespace SeedFinder
