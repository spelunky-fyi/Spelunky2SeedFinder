#include "FilterFindRegularShopWithSpecificContents.h"
#include "SeedFinder.h"

namespace SeedFinder
{
    const char* FilterFindRegularShopWithSpecificContents::msComboItemOptions[] = {
        "",                    // 0
        "Axolotl",             // 1
        "Bomb Bag",            // 2
        "Bomb Box",            // 3
        "Boomerang",           // 4
        "Camera",              // 5
        "Cape",                // 6
        "Climbing Gloves",     // 7
        "Compass",             // 8
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
        "Powerpack",           // 23
        "Present",             // 24
        "Rockdog",             // 25
        "Rope Pile",           // 26
        "Shotgun",             // 27
        "Skeleton Key",        // 28
        "Spectacles",          // 29
        "Spike Shoes",         // 30
        "Spring Shoes",        // 31
        "Teleporter",          // 32
        "Teleporter Backpack", // 33
        "Turkey",              // 34
        "Web Gun"              // 35
    };
    size_t FilterFindRegularShopWithSpecificContents::msShopItemIDs[cShopItemsCount] = {0};

    FilterFindRegularShopWithSpecificContents::FilterFindRegularShopWithSpecificContents(SeedFinder* seedFinder) : FilterFindShopWithSpecificContents(seedFinder)
    {
        if (msShopItemIDs[1] == 0)
        {
            msShopItemIDs[0] = 0;
            msShopItemIDs[1] = to_id("ENT_TYPE_MOUNT_AXOLOTL");
            msShopItemIDs[2] = to_id("ENT_TYPE_ITEM_PICKUP_BOMBBAG");
            msShopItemIDs[3] = to_id("ENT_TYPE_ITEM_PICKUP_BOMBBOX");
            msShopItemIDs[4] = to_id("ENT_TYPE_ITEM_BOOMERANG");
            msShopItemIDs[5] = to_id("ENT_TYPE_ITEM_CAMERA");
            msShopItemIDs[6] = to_id("ENT_TYPE_ITEM_PURCHASABLE_CAPE");
            msShopItemIDs[7] = to_id("ENT_TYPE_ITEM_PICKUP_CLIMBINGGLOVES");
            msShopItemIDs[8] = to_id("ENT_TYPE_ITEM_PICKUP_COMPASS");
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
            msShopItemIDs[23] = to_id("ENT_TYPE_ITEM_PURCHASABLE_POWERPACK");
            msShopItemIDs[24] = to_id("ENT_TYPE_ITEM_PRESENT");
            msShopItemIDs[25] = to_id("ENT_TYPE_MOUNT_ROCKDOG");
            msShopItemIDs[26] = to_id("ENT_TYPE_ITEM_PICKUP_ROPEPILE");
            msShopItemIDs[27] = to_id("ENT_TYPE_ITEM_SHOTGUN");
            msShopItemIDs[28] = to_id("ENT_TYPE_ITEM_PICKUP_SKELETON_KEY");
            msShopItemIDs[29] = to_id("ENT_TYPE_ITEM_PICKUP_SPECTACLES");
            msShopItemIDs[30] = to_id("ENT_TYPE_ITEM_PICKUP_SPIKESHOES");
            msShopItemIDs[31] = to_id("ENT_TYPE_ITEM_PICKUP_SPRINGSHOES");
            msShopItemIDs[32] = to_id("ENT_TYPE_ITEM_TELEPORTER");
            msShopItemIDs[33] = to_id("ENT_TYPE_ITEM_PURCHASABLE_TELEPORTER_BACKPACK");
            msShopItemIDs[34] = to_id("ENT_TYPE_MOUNT_TURKEY");
            msShopItemIDs[35] = to_id("ENT_TYPE_ITEM_WEBGUN");
        }
        mLevelsToSearch.disableLevel(1, 1);
        mLevelsToSearch.disableLevel(1, 4);
        mLevelsToSearch.disableLevel(3, 1);
        mLevelsToSearch.disableLevel(6, 4);
        mLevelsToSearch.disableLevel(7, 4);
    }

    std::string FilterFindRegularShopWithSpecificContents::title() { return "Find regular shop with specific contents"; }

    std::string FilterFindRegularShopWithSpecificContents::filterTitle() const { return FilterFindRegularShopWithSpecificContents::title(); }

    size_t FilterFindRegularShopWithSpecificContents::itemPositionsInShop() const { return 4; }

    size_t FilterFindRegularShopWithSpecificContents::itemPossibilitiesInShop() const { return cShopItemsCount; }

    size_t FilterFindRegularShopWithSpecificContents::shopItemID(int8_t index) const { return msShopItemIDs[index]; }

    const char* FilterFindRegularShopWithSpecificContents::comboItemOption(size_t index) const { return msComboItemOptions[index]; }

    size_t FilterFindRegularShopWithSpecificContents::shopKeeperID() const
    {
        static size_t identifier = 0;
        if (identifier == 0)
        {
            identifier = to_id("ENT_TYPE_MONS_SHOPKEEPER");
        }
        return identifier;
    }

    LayerChoice* FilterFindRegularShopWithSpecificContents::searchInLayer() { return &mLayer; }

    bool FilterFindRegularShopWithSpecificContents::showLayerCombo() const { return true; }
} // namespace SeedFinder