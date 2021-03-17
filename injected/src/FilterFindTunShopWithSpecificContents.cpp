#include "FilterFindTunShopWithSpecificContents.h"
#include "SeedFinder.h"

namespace SeedFinder
{
    const char* FilterFindTunShopWithSpecificContents::msComboItemOptions[] = {
        "",                    // 0
        "Bomb Bag",            // 1
        "Bomb Box",            // 2
        "Boomerang",           // 3
        "Camera",              // 4
        "Cape",                // 5
        "Climbing Gloves",     // 6
        "Compass",             // 7
        "Crossbow",            // 8
        "Freeze Ray",          // 9
        "Hired Hand",          // 10
        "Hoverpack",           // 11
        "Jetpack",             // 12
        "Machete",             // 13
        "Mattock",             // 14
        "Metal Shield",        // 15
        "Parachute",           // 16
        "Paste",               // 17
        "Pet (Cat)",           // 18
        "Pet (Dog)",           // 19
        "Pet (Hamster)",       // 20
        "Pitcher's Mitt",      // 21
        "Powerpack",           // 22
        "Plasma Cannon",       // 23
        "Rope Pile",           // 24
        "Royal Jelly",         // 25
        "Shotgun",             // 26
        "Spectacles",          // 27
        "Spike Shoes",         // 28
        "Spring Shoes",        // 29
        "Teleporter",          // 30
        "Teleporter Backpack", // 31
        "Web Gun"              // 32
    };
    size_t FilterFindTunShopWithSpecificContents::msShopItemIDs[cShopItemsCount] = {0};

    FilterFindTunShopWithSpecificContents::FilterFindTunShopWithSpecificContents(SeedFinder* seedFinder) : FilterFindShopWithSpecificContents(seedFinder)
    {
        if (msShopItemIDs[1] == 0)
        {
            msShopItemIDs[0] = 0;
            msShopItemIDs[1] = to_id("ENT_TYPE_ITEM_PICKUP_BOMBBAG");
            msShopItemIDs[2] = to_id("ENT_TYPE_ITEM_PICKUP_BOMBBOX");
            msShopItemIDs[3] = to_id("ENT_TYPE_ITEM_BOOMERANG");
            msShopItemIDs[4] = to_id("ENT_TYPE_ITEM_CAMERA");
            msShopItemIDs[5] = to_id("ENT_TYPE_ITEM_PURCHASABLE_CAPE");
            msShopItemIDs[6] = to_id("ENT_TYPE_ITEM_PICKUP_CLIMBINGGLOVES");
            msShopItemIDs[7] = to_id("ENT_TYPE_ITEM_PICKUP_COMPASS");
            msShopItemIDs[8] = to_id("ENT_TYPE_ITEM_CROSSBOW");
            msShopItemIDs[9] = to_id("ENT_TYPE_ITEM_FREEZERAY");
            msShopItemIDs[10] = to_id("ENT_TYPE_CHAR_HIREDHAND");
            msShopItemIDs[11] = to_id("ENT_TYPE_ITEM_PURCHASABLE_HOVERPACK");
            msShopItemIDs[12] = to_id("ENT_TYPE_ITEM_PURCHASABLE_JETPACK");
            msShopItemIDs[13] = to_id("ENT_TYPE_ITEM_MACHETE");
            msShopItemIDs[14] = to_id("ENT_TYPE_ITEM_MATTOCK");
            msShopItemIDs[15] = to_id("ENT_TYPE_ITEM_METAL_SHIELD");
            msShopItemIDs[16] = to_id("ENT_TYPE_ITEM_PICKUP_PARACHUTE");
            msShopItemIDs[17] = to_id("ENT_TYPE_ITEM_PICKUP_PASTE");
            msShopItemIDs[18] = to_id("ENT_TYPE_MONS_PET_CAT");
            msShopItemIDs[19] = to_id("ENT_TYPE_MONS_PET_DOG");
            msShopItemIDs[20] = to_id("ENT_TYPE_MONS_PET_HAMSTER");
            msShopItemIDs[21] = to_id("ENT_TYPE_ITEM_PICKUP_PITCHERSMITT");
            msShopItemIDs[22] = to_id("ENT_TYPE_ITEM_PURCHASABLE_POWERPACK");
            msShopItemIDs[23] = to_id("ENT_TYPE_ITEM_PLASMACANNON");
            msShopItemIDs[24] = to_id("ENT_TYPE_ITEM_PICKUP_ROPEPILE");
            msShopItemIDs[25] = to_id("ENT_TYPE_ITEM_PICKUP_ROYALJELLY");
            msShopItemIDs[26] = to_id("ENT_TYPE_ITEM_SHOTGUN");
            msShopItemIDs[27] = to_id("ENT_TYPE_ITEM_PICKUP_SPECTACLES");
            msShopItemIDs[28] = to_id("ENT_TYPE_ITEM_PICKUP_SPIKESHOES");
            msShopItemIDs[29] = to_id("ENT_TYPE_ITEM_PICKUP_SPRINGSHOES");
            msShopItemIDs[30] = to_id("ENT_TYPE_ITEM_TELEPORTER");
            msShopItemIDs[31] = to_id("ENT_TYPE_ITEM_TELEPORTER_BACKPACK");
            msShopItemIDs[32] = to_id("ENT_TYPE_ITEM_WEBGUN");
        }
    }

    std::string FilterFindTunShopWithSpecificContents::uniqueIdentifier()
    {
        return "FilterFindTunShopWithSpecificContents";
    }

    std::string FilterFindTunShopWithSpecificContents::title()
    {
        return "Find Tun shop with specific contents";
    }

    std::unique_ptr<FilterFindTunShopWithSpecificContents> FilterFindTunShopWithSpecificContents::instantiate(SeedFinder* seedFinder)
    {
        return (std::make_unique<FilterFindTunShopWithSpecificContents>(seedFinder));
    }

    std::string FilterFindTunShopWithSpecificContents::filterTitle() const
    {
        return FilterFindTunShopWithSpecificContents::title();
    }

    size_t FilterFindTunShopWithSpecificContents::itemPositionsInShop() const
    {
        return 3;
    }

    size_t FilterFindTunShopWithSpecificContents::itemPossibilitiesInShop() const
    {
        return cShopItemsCount;
    }

    size_t FilterFindTunShopWithSpecificContents::shopItemID(int8_t index) const
    {
        return msShopItemIDs[index];
    }

    const char* FilterFindTunShopWithSpecificContents::comboItemOption(size_t index) const
    {
        return msComboItemOptions[index];
    }

    size_t FilterFindTunShopWithSpecificContents::shopKeeperID() const
    {
        static size_t identifier = 0;
        if (identifier == 0)
        {
            identifier = to_id("ENT_TYPE_MONS_MERCHANT");
        }
        return identifier;
    }

    LayerChoice* FilterFindTunShopWithSpecificContents::searchInLayer()
    {
        return &mLayer;
    }

    bool FilterFindTunShopWithSpecificContents::showLayerCombo() const
    {
        return false;
    }

    json FilterFindTunShopWithSpecificContents::serialize() const
    {
        auto j = FilterFindShopWithSpecificContents::serialize();
        j[SeedFinder::kJSONVersion] = 1;
        j[SeedFinder::kJSONFilterID] = uniqueIdentifier();
        j[SeedFinder::kJSONLayer] = static_cast<int>(mLayer);
        return j;
    }

    std::string FilterFindTunShopWithSpecificContents::unserialize(const json& j)
    {
        FilterFindShopWithSpecificContents::unserialize(j);
        if (j.contains(SeedFinder::kJSONVersion))
        {
            auto version = j.at(SeedFinder::kJSONVersion).get<uint8_t>();
            if (version == 1)
            {
                static const std::vector<const char*> items = {kJSONItem1, kJSONItem2, kJSONItem3, kJSONItem4};
                uint8_t counter = 0;
                for (const auto& itemNumber : items)
                {
                    if (j.contains(itemNumber))
                    {
                        auto idString = j.at(itemNumber).get<std::string>();
                        auto id = to_id(idString);
                        for (auto x = 0; x < cShopItemsCount; ++x)
                        {
                            if (id == msShopItemIDs[x])
                            {
                                mItemIDs[counter] = id;
                                mComboChosenItemIDs[counter] = msComboItemOptions[x];
                            }
                        }
                    }
                    counter++;
                }

                if (j.contains(SeedFinder::kJSONLayer))
                {
                    mLayer = static_cast<LayerChoice>(j.at(SeedFinder::kJSONLayer).get<uint8_t>());
                }
            }
            else
            {
                return fmt::format("Version mismatch for {}, can't read this version", uniqueIdentifier());
            }
        }
        else
        {
            return fmt::format("No version number specified for {}", uniqueIdentifier());
        }
        return "";
    }

} // namespace SeedFinder