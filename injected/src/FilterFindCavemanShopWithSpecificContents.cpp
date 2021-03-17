#include "FilterFindCavemanShopWithSpecificContents.h"
#include "SeedFinder.h"

namespace SeedFinder
{
    const char* FilterFindCavemanShopWithSpecificContents::msComboItemOptions[] = {
        "",                    // 0
        "Bomb Bag",            // 1
        "Bomb Box",            // 2
        "Boomerang",           // 3
        "Camera",              // 4
        "Cape",                // 5
        "Climbing Gloves",     // 6
        "Compass",             // 7
        "Cooked Turkey",       // 8
        "Crossbow",            // 9
        "Freeze Ray",          // 10
        "Hired Hand",          // 11
        "Hoverpack",           // 12
        "Jetpack",             // 13
        "Machete",             // 14
        "Mattock",             // 15
        "Metal Shield",        // 16
        "Parachute",           // 17
        "Paste",               // 18
        "Pet (Cat)",           // 19
        "Pet (Dog)",           // 20
        "Pet (Hamster)",       // 21
        "Pitcher's Mitt",      // 22
        "Pot",                 // 23
        "Powerpack",           // 24
        "Plasma Cannon",       // 25
        "Rock",                // 26
        "Rope Pile",           // 27
        "Royal Jelly",         // 28
        "Shotgun",             // 29
        "Skull",               // 30
        "Spectacles",          // 31
        "Spike Shoes",         // 32
        "Spring Shoes",        // 33
        "Teleporter",          // 34
        "Teleporter Backpack", // 35
        "Web Gun",             // 36
        "Wooden Arrow"         // 37
    };
    size_t FilterFindCavemanShopWithSpecificContents::msShopItemIDs[cShopItemsCount] = {0};

    FilterFindCavemanShopWithSpecificContents::FilterFindCavemanShopWithSpecificContents(SeedFinder* seedFinder) : FilterFindShopWithSpecificContents(seedFinder)
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
            msShopItemIDs[8] = to_id("ENT_TYPE_ITEM_PICKUP_COOKEDTURKEY");
            msShopItemIDs[9] = to_id("ENT_TYPE_ITEM_CROSSBOW");
            msShopItemIDs[10] = to_id("ENT_TYPE_ITEM_FREEZERAY");
            msShopItemIDs[11] = to_id("ENT_TYPE_CHAR_HIREDHAND");
            msShopItemIDs[12] = to_id("ENT_TYPE_ITEM_PURCHASABLE_HOVERPACK");
            msShopItemIDs[13] = to_id("ENT_TYPE_ITEM_PURCHASABLE_JETPACK");
            msShopItemIDs[14] = to_id("ENT_TYPE_ITEM_MACHETE");
            msShopItemIDs[15] = to_id("ENT_TYPE_ITEM_MATTOCK");
            msShopItemIDs[16] = to_id("ENT_TYPE_ITEM_METAL_SHIELD");
            msShopItemIDs[17] = to_id("ENT_TYPE_ITEM_PICKUP_PARACHUTE");
            msShopItemIDs[18] = to_id("ENT_TYPE_ITEM_PICKUP_PASTE");
            msShopItemIDs[19] = to_id("ENT_TYPE_MONS_PET_CAT");
            msShopItemIDs[20] = to_id("ENT_TYPE_MONS_PET_DOG");
            msShopItemIDs[21] = to_id("ENT_TYPE_MONS_PET_HAMSTER");
            msShopItemIDs[22] = to_id("ENT_TYPE_ITEM_PICKUP_PITCHERSMITT");
            msShopItemIDs[23] = to_id("ENT_TYPE_ITEM_POT");
            msShopItemIDs[24] = to_id("ENT_TYPE_ITEM_PURCHASABLE_POWERPACK");
            msShopItemIDs[25] = to_id("ENT_TYPE_ITEM_PLASMACANNON");
            msShopItemIDs[26] = to_id("ENT_TYPE_ITEM_ROCK");
            msShopItemIDs[27] = to_id("ENT_TYPE_ITEM_PICKUP_ROPEPILE");
            msShopItemIDs[28] = to_id("ENT_TYPE_ITEM_PICKUP_ROYALJELLY");
            msShopItemIDs[29] = to_id("ENT_TYPE_ITEM_SHOTGUN");
            msShopItemIDs[30] = to_id("ENT_TYPE_ITEM_SKULL");
            msShopItemIDs[31] = to_id("ENT_TYPE_ITEM_PICKUP_SPECTACLES");
            msShopItemIDs[32] = to_id("ENT_TYPE_ITEM_PICKUP_SPIKESHOES");
            msShopItemIDs[33] = to_id("ENT_TYPE_ITEM_PICKUP_SPRINGSHOES");
            msShopItemIDs[34] = to_id("ENT_TYPE_ITEM_TELEPORTER");
            msShopItemIDs[35] = to_id("ENT_TYPE_ITEM_TELEPORTER_BACKPACK");
            msShopItemIDs[36] = to_id("ENT_TYPE_ITEM_WEBGUN");
            msShopItemIDs[37] = to_id("ENT_TYPE_ITEM_WOODEN_ARROW");
        }
    }

    std::string FilterFindCavemanShopWithSpecificContents::uniqueIdentifier()
    {
        return "FilterFindCavemanShopWithSpecificContents";
    }

    std::string FilterFindCavemanShopWithSpecificContents::title()
    {
        return "Find caveman shop with specific contents";
    }

    std::unique_ptr<FilterFindCavemanShopWithSpecificContents> FilterFindCavemanShopWithSpecificContents::instantiate(SeedFinder* seedFinder)
    {
        return (std::make_unique<FilterFindCavemanShopWithSpecificContents>(seedFinder));
    }

    std::string FilterFindCavemanShopWithSpecificContents::filterTitle() const
    {
        return FilterFindCavemanShopWithSpecificContents::title();
    }

    size_t FilterFindCavemanShopWithSpecificContents::itemPositionsInShop() const
    {
        return 3;
    }

    size_t FilterFindCavemanShopWithSpecificContents::itemPossibilitiesInShop() const
    {
        return cShopItemsCount;
    }

    size_t FilterFindCavemanShopWithSpecificContents::shopItemID(int8_t index) const
    {
        return msShopItemIDs[index];
    }

    const char* FilterFindCavemanShopWithSpecificContents::comboItemOption(size_t index) const
    {
        return msComboItemOptions[index];
    }

    size_t FilterFindCavemanShopWithSpecificContents::shopKeeperID() const
    {
        static size_t identifier = 0;
        if (identifier == 0)
        {
            identifier = to_id("ENT_TYPE_MONS_CAVEMAN_SHOPKEEPER");
        }
        return identifier;
    }

    LayerChoice* FilterFindCavemanShopWithSpecificContents::searchInLayer()
    {
        return &mLayer;
    }

    bool FilterFindCavemanShopWithSpecificContents::showLayerCombo() const
    {
        return false;
    }

    json FilterFindCavemanShopWithSpecificContents::serialize() const
    {
        auto j = FilterFindShopWithSpecificContents::serialize();
        j[SeedFinder::kJSONVersion] = 1;
        j[SeedFinder::kJSONFilterID] = uniqueIdentifier();
        j[SeedFinder::kJSONLayer] = static_cast<int>(mLayer);
        return j;
    }

    std::string FilterFindCavemanShopWithSpecificContents::unserialize(const json& j)
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