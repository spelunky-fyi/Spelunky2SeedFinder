#include "SeedFinder.h"
#include "FilterFindAltar.h"
#include "FilterFindAnkhSkip.h"
#include "FilterFindBlackMarket.h"
#include "FilterFindCavemanShopWithSpecificContents.h"
#include "FilterFindCrateWithSpecificContents.h"
#include "FilterFindDiceShop.h"
#include "FilterFindEntity.h"
#include "FilterFindGhostJar.h"
#include "FilterFindItemInCrust.h"
#include "FilterFindLevelFeelings.h"
#include "FilterFindMoonChallenge.h"
#include "FilterFindPet.h"
#include "FilterFindPotWithSpecificContents.h"
#include "FilterFindQuillbackObstruction.h"
#include "FilterFindRegularShopWithSpecificContents.h"
#include "FilterFindSisters.h"
#include "FilterFindSunChallenge.h"
#include "FilterFindTunShopWithSpecificContents.h"
#include "FilterFindUdjatEye.h"
#include "FilterFindVanHorsing.h"
#include "FilterFindVault.h"
#include "FilterFindVolcanaDrill.h"
#include "entity.hpp"
#include "rpc.hpp"
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

#ifdef SEEDFINDER_WARPTEST
#include "sha256.h"
#endif

namespace SeedFinder
{
    struct StateMemory* g_state = nullptr;
    uint16_t SeedFinder::mEggplantMotherStatueHandID = 0;

    SeedFinder::SeedFinder()
    {
        registerFilterTitles();
    }

    void SeedFinder::registerAllEntities(const std::vector<EntityItem>& entities)
    {
        g_state = State::get().ptr();
        mAllEntities = entities;
        std::sort(mAllEntities.begin(), mAllEntities.end(), [](const EntityItem& a, const EntityItem& b) -> bool { return (a.name.compare(b.name) < 0); });
    }

    const std::vector<EntityItem>& SeedFinder::allEntities()
    {
        return mAllEntities;
    }

    uint32_t SeedFinder::currentSeed() const noexcept
    {
        return mCurrentSeed;
    }

    void SeedFinder::registerFilterTitles()
    {
        mFilterTitles.emplace_back(FilterFindCrateWithSpecificContents::title());
        mFilterTitles.emplace_back(FilterFindRegularShopWithSpecificContents::title());
        mFilterTitles.emplace_back(FilterFindTunShopWithSpecificContents::title());
        mFilterTitles.emplace_back(FilterFindCavemanShopWithSpecificContents::title());
        mFilterTitles.emplace_back(FilterFindAltar::title());
        mFilterTitles.emplace_back(FilterFindUdjatEye::title());
        mFilterTitles.emplace_back(FilterFindVolcanaDrill::title());
        mFilterTitles.emplace_back(FilterFindBlackMarket::title());
        mFilterTitles.emplace_back(FilterFindQuillbackObstruction::title());
        mFilterTitles.emplace_back(FilterFindItemInCrust::title());
        mFilterTitles.emplace_back(FilterFindVault::title());
        mFilterTitles.emplace_back(FilterFindDiceShop::title());
        mFilterTitles.emplace_back(FilterFindLevelFeelings::title());
        mFilterTitles.emplace_back(FilterFindPet::title());
        mFilterTitles.emplace_back(FilterFindEntity::title());
        mFilterTitles.emplace_back(FilterFindMoonChallenge::title());
        mFilterTitles.emplace_back(FilterFindSunChallenge::title());
        mFilterTitles.emplace_back(FilterFindGhostJar::title());
        mFilterTitles.emplace_back(FilterFindSisters::title());
        mFilterTitles.emplace_back(FilterFindPotWithSpecificContents::title());
        mFilterTitles.emplace_back(FilterFindAnkhSkip::title());
        mFilterTitles.emplace_back(FilterFindVanHorsing::title());

        std::sort(mFilterTitles.begin(), mFilterTitles.end(), [](const std::string& a, const std::string& b) -> bool { return (_stricmp(a.c_str(), b.c_str()) < 0); });
    }

    void SeedFinder::addFilterByTitle(const char* filterTitle)
    {
        if (filterTitle == FilterFindCrateWithSpecificContents::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindCrateWithSpecificContents>(this));
        }
        else if (filterTitle == FilterFindRegularShopWithSpecificContents::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindRegularShopWithSpecificContents>(this));
        }
        else if (filterTitle == FilterFindTunShopWithSpecificContents::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindTunShopWithSpecificContents>(this));
        }
        else if (filterTitle == FilterFindCavemanShopWithSpecificContents::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindCavemanShopWithSpecificContents>(this));
        }
        else if (filterTitle == FilterFindAltar::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindAltar>(this));
        }
        else if (filterTitle == FilterFindUdjatEye::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindUdjatEye>(this));
        }
        else if (filterTitle == FilterFindVolcanaDrill::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindVolcanaDrill>(this));
        }
        else if (filterTitle == FilterFindBlackMarket::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindBlackMarket>(this));
        }
        else if (filterTitle == FilterFindQuillbackObstruction::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindQuillbackObstruction>(this));
        }
        else if (filterTitle == FilterFindItemInCrust::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindItemInCrust>(this));
        }
        else if (filterTitle == FilterFindVault::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindVault>(this));
        }
        else if (filterTitle == FilterFindDiceShop::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindDiceShop>(this));
        }
        else if (filterTitle == FilterFindLevelFeelings::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindLevelFeelings>(this));
        }
        else if (filterTitle == FilterFindPet::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindPet>(this));
        }
        else if (filterTitle == FilterFindEntity::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindEntity>(this));
        }
        else if (filterTitle == FilterFindMoonChallenge::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindMoonChallenge>(this));
        }
        else if (filterTitle == FilterFindSunChallenge::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindSunChallenge>(this));
        }
        else if (filterTitle == FilterFindGhostJar::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindGhostJar>(this));
        }
        else if (filterTitle == FilterFindSisters::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindSisters>(this));
        }
        else if (filterTitle == FilterFindPotWithSpecificContents::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindPotWithSpecificContents>(this));
        }
        else if (filterTitle == FilterFindAnkhSkip::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindAnkhSkip>(this));
        }
        else if (filterTitle == FilterFindVanHorsing::title())
        {
            mFilters.emplace_back(std::make_unique<FilterFindVanHorsing>(this));
        }
    }

    void SeedFinder::render()
    {
        if (mSeedFinderState != SeedFinderState::IDLE || (g_state->ingame && g_state->playing))
        {
            bool allFiltersValid = areAllFiltersValid();
            // ************* CONFIGURATION *************
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Text("Configuration");

            ImGui::SetCursorPosX(gSectionMarginHor);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Amount of seeds to process :");
            ImGui::SameLine();
            ImGui::PushItemWidth(100);
            ImGui::InputScalar("##SeedFinderAmountOfSeeds", ImGuiDataType_U32, &mAmountToProcess);

            ImGui::SetCursorPosX(gSectionMarginHor);
            ImGui::Checkbox("Generate random seeds, or start at seed:##SeedFinderGenerateRandomSeeds", &mUseRandomSeeds);
            ImGui::SameLine();
            ImGui::InputScalar("##SeedFinderStartAtSeed", ImGuiDataType_U32, &msStartSeed, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal | (mUseRandomSeeds ? ImGuiInputTextFlags_ReadOnly : 0));

            ImGui::SetCursorPosX(gSectionMarginHor);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Anti flicker frames :");
            ImGui::SameLine();
            ImGui::InputScalar("##SeedFinderAntiFlickerFrameCount", ImGuiDataType_U32, &mAntiFlickerFrameCount);

            auto deepestLevel = deepestLevelOfAllFilters();
            if (mGoToAbzu)
            {
                mGoToCityOfGold = false;
                mGoToDuat = false;
                mGoToTidePool = true;
            }
            if (mGoToCityOfGold || mGoToDuat)
            {
                mGoToAbzu = false;
                mGoToTidePool = false;
                if (mGoToDuat)
                {
                    mGoToCityOfGold = true;
                }
            }

            if (deepestLevel > gThresholdChooseJungleVolcana)
            {
                ImGui::SetCursorPosX(gSectionMarginHor);
                ImGui::Text("Go to :");
                ImGui::SameLine();
                if (ImGui::RadioButton("Jungle##SeedFinderGoToJungle", mGoToJungle))
                {
                    mGoToJungle = true;
                }
                ImGui::SameLine();
                if (ImGui::RadioButton("Volcana##SeedFinderGoToVolcana", !mGoToJungle))
                {
                    mGoToJungle = false;
                }
            }
            if (deepestLevel > gThresholdChooseTidePoolTemple)
            {
                ImGui::SetCursorPosX(gSectionMarginHor);
                ImGui::Text("Go to :");
                ImGui::SameLine();
                if (ImGui::RadioButton("Tide Pool##SeedFinderGoToTidePool", mGoToTidePool))
                {
                    mGoToCityOfGold = false;
                    mGoToDuat = false;
                    mGoToTidePool = true;
                }
                ImGui::SameLine();
                ImGui::Checkbox("Abzu##SeedFinderGoToAbzu", &mGoToAbzu);

                ImGui::SameLine();
                if (ImGui::RadioButton("Temple", !mGoToTidePool))
                {
                    mGoToAbzu = false;
                    mGoToTidePool = false;
                }
                ImGui::SameLine();
                ImGui::Checkbox("City of Gold##SeedFinderGoToCityOfGold", &mGoToCityOfGold);
                ImGui::SameLine();
                ImGui::Checkbox("Duat##SeedFinderGoToDuat", &mGoToDuat);
            }
            if (deepestLevel > gThresholdChooseEggplantWorld)
            {
                ImGui::SetCursorPosX(gSectionMarginHor);
                ImGui::Text("Go to :");
                ImGui::SameLine();
                ImGui::Checkbox("Eggplant World##SeedFinderGoToEggplantWorld", &mGoToEggplantWorld);
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
            ImGui::Separator();
            ImGui::Separator();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

            // ************* FILTERS *************
            ImGui::Text("Filter(s)");
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

            for (const auto& filter : mFilters)
            {
                filter->render();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                auto error = filter->lastError();
                if (!error.empty())
                {
                    ImGui::SetCursorPosX(gSectionMarginHor * 2);
                    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), error.c_str());
                }
            }
            if (mFilters.size() > 0)
            {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            }

            ImGui::SetCursorPosX(gSectionMarginHor);
            ImGui::PushItemWidth(250);
            if (ImGui::BeginCombo("##SeedFinderComboAddFilter", mComboAddFilterChosenTitle))
            {
                for (auto i = 0; i < mFilterTitles.size(); ++i)
                {
                    bool isSelected = (mComboAddFilterChosenTitle == mFilterTitles.at(i).c_str());
                    if (ImGui::Selectable(mFilterTitles.at(i).c_str(), isSelected))
                    {
                        mComboAddFilterChosenTitle = mFilterTitles.at(i).c_str();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            if (ImGui::Button("Add filter##SeedFinderAddFilter"))
            {
                addFilterByTitle(mComboAddFilterChosenTitle);
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
            ImGui::Separator();
            ImGui::Separator();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

            // ************* START BUTTON *************
            if (!allFiltersValid)
            {
                const char* caption = "Please configure the filters to search";
                ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x / 2.) - (ImGui::CalcTextSize(caption).x / 2.), ImGui::GetCursorPosY() + 10));
                ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), caption);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            }
            else
            {
                if (mSeedFinderState == SeedFinderState::IDLE)
                {
                    static const float buttonWidth = 80;
                    ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x / 2.) - (buttonWidth / 2.), ImGui::GetCursorPosY() + 10));
                    if (ImGui::Button("Find Seeds##SeedFinderFindSeeds", ImVec2(buttonWidth, 0)))
                    {
                        pause(30, SeedFinderState::INIT);
                    }
                }
                else
                {
                    static const float buttonWidth = 80;
                    ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x / 2.) - (buttonWidth / 2.), ImGui::GetCursorPosY() + 10));
                    if (ImGui::Button("Cancel##SeedFinderCancel", ImVec2(buttonWidth, 0)))
                    {
                        Util::log("Cancelled");
                        mSeedFinderState = SeedFinderState::CLEANUP;
                    }
                }
            }

#ifdef SEEDFINDER_WARPTEST
            static const float buttonWidth = 80;
            ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x / 2.) - (buttonWidth / 2.), ImGui::GetCursorPosY() + 10));
            if (ImGui::Button("Warp Test##SeedFinderWarpTestButton", ImVec2(buttonWidth, 0)))
            {
                warpTest();
            }
#endif

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
            ImGui::Separator();
            ImGui::Separator();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

            // ************* FOUND SEEDS *************
            ImGui::Text("Found seed(s)");

            static const uint32_t seedButtonWidth = 90;
            static const uint32_t seedButtonMargin = 10;

            if (mSeedFinderState == SeedFinderState::EXECUTE_FILTERS || mSeedFinderState == SeedFinderState::EXECUTE_LOAD_SEED || mSeedFinderState == SeedFinderState::PAUSE)
            {
                const char* caption = fmt::format("Processing seed {:08X}...", mCurrentSeed).c_str();
                ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x / 2.) - (ImGui::CalcTextSize(caption).x / 2.), ImGui::GetCursorPosY()));
                ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), caption);
                caption = "Spelunky will freeze while searching, please be patient";
                ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x / 2.) - (ImGui::CalcTextSize(caption).x / 2.), ImGui::GetCursorPosY()));
                ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), caption);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            }
            else if (mFoundSeeds.empty())
            {
                const char* caption = "No seeds found...";
                ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x / 2.) - (ImGui::CalcTextSize(caption).x / 2.), ImGui::GetCursorPosY()));
                ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), caption);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            }
            else
            {
                uint32_t x = gSectionMarginHor;
                bool first = true;
                for (const auto& foundSeed : mFoundSeeds)
                {
                    if (!first)
                    {
                        ImGui::SameLine();
                    }
                    if (first || !((ImGui::GetCursorPosX() + seedButtonWidth) < (ImGui::GetWindowSize().x - gSectionMarginHor)))
                    {
                        ImGui::NewLine();
                        ImGui::SetCursorPosX(gSectionMarginHor);
                    }
                    first = false;

                    if (ImGui::Button(fmt::format("{:08X}##SeedFinderLoadSeed{}", foundSeed, foundSeed).c_str(), ImVec2(seedButtonWidth, 0)))
                    {
                        setGameSeed(foundSeed);
                    }
                }
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            }

            handleState();

            // clean up deleted filters
            mFilters.erase(std::remove_if(mFilters.begin(), mFilters.end(), [](const std::unique_ptr<Filter>& f) { return f->isDeleted(); }), mFilters.end());
        }
        else
        {
            const char* caption = "To search for seeds, please make sure you are in an actual level.";
            ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x / 2.) - (ImGui::CalcTextSize(caption).x / 2.), ImGui::GetCursorPosY() + 40));
            ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), caption);
        }
    }

    void SeedFinder::setGameSeed(uint32_t seed)
    {
        if (g_state != nullptr)
        {
            auto memory = Memory::get();

            // RevEng: Set hw write bp on state->feedcode, enter a seed in game and start the level
            typedef void sgs_func(uint32_t dummy, uint32_t seed);
            static sgs_func* sgs = (sgs_func*)(memory.at_exe(0x221301C0));

            // Set the "screen_last" state to 0x0E, so the game thinks we came from the
            // "Enter New Seed" menu item in-game. This way, the character selection
            // screen is not shown, and we are immediately in game.
            g_state->screen_last = 0x0E;
            // First param is probably *this, but is not used in the function
            sgs(0, seed);
        }
    };

    void SeedFinder::warp(uint8_t world, uint8_t level, uint8_t theme)
    {
        if (g_state != nullptr)
        {
            auto memory = Memory::get();

            typedef void w_func(struct StateMemory*, int);
            static w_func* w = (w_func*)(memory.at_exe(0x221A7080));

            g_state->world = world;
            g_state->world_next = world;
            g_state->level = level;
            g_state->level_next = level;
            g_state->theme = theme;
            g_state->theme_next = theme;
            w(g_state, 0);
        }
    };

    uint32_t SeedFinder::nextSeed(uint32_t currentSeed)
    {
        if (mUseRandomSeeds)
        {
            std::random_device r;
            std::default_random_engine e(r());
            std::uniform_int_distribution<int> uniform_dist(0, 0xFFFFFFFF);
            return uniform_dist(e);
        }
        else
        {
            return currentSeed + 1;
        }
    };

    uint8_t SeedFinder::deepestLevelOfAllFilters()
    {
        uint8_t deepestLevel = 0;
        for (const auto& filter : mFilters)
        {
            auto deepestLevelOfFilter = filter->deepestLevel();
            if (deepestLevelOfFilter > deepestLevel)
            {
                deepestLevel = deepestLevelOfFilter;
            }
        }
        return deepestLevel;
    }

    bool SeedFinder::areAllFiltersValid()
    {
        if (mFilters.empty())
        {
            return false;
        }
        bool allFiltersValid = true;
        for (const auto& filter : mFilters)
        {
            if (!filter->isValid())
            {
                allFiltersValid = false; // can't return early because all filters must be checked, so they can put an error message in case of invalidity
            }
        }
        return allFiltersValid;
    }

    WarpInfo SeedFinder::warpInfoForDepth(uint8_t d)
    {
        switch (d)
        {
        case 0:
            return WarpInfo(1, 1, THEME_DWELLING);
        case 1:
            return WarpInfo(1, 2, THEME_DWELLING);
        case 2:
            return WarpInfo(1, 3, THEME_DWELLING);
        case 3:
            return WarpInfo(1, 4, THEME_DWELLING);
        case 4:
            return WarpInfo(2, 1, mGoToJungle ? THEME_JUNGLE : THEME_VOLCANA);
        case 5:
            return WarpInfo(2, 2, mGoToJungle ? THEME_JUNGLE : THEME_VOLCANA);
        case 6:
            return WarpInfo(2, 3, mGoToJungle ? THEME_JUNGLE : THEME_VOLCANA);
        case 7:
            return WarpInfo(2, 4, mGoToJungle ? THEME_JUNGLE : THEME_VOLCANA);
        case 8:
            return WarpInfo(3, 1, THEME_OLMEC);
        case 9:
            return WarpInfo(4, 1, mGoToTidePool ? THEME_TIDE_POOL : THEME_TEMPLE);
        case 10:
            return WarpInfo(4, 2, mGoToTidePool ? THEME_TIDE_POOL : THEME_TEMPLE);
        case 11:
            return WarpInfo(4, 3, mGoToTidePool ? THEME_TIDE_POOL : (mGoToCityOfGold ? THEME_CITY_OF_GOLD : THEME_TEMPLE));
        case 12:
            return WarpInfo(4, 4, mGoToTidePool ? (mGoToAbzu ? THEME_ABZU : THEME_TIDE_POOL) : (mGoToDuat ? THEME_DUAT : THEME_TEMPLE));
        case 13:
            return WarpInfo(5, 1, THEME_ICE_CAVES);
        case 14:
            return WarpInfo(6, 1, THEME_NEO_BABYLON);
        case 15:
            return WarpInfo(6, 2, THEME_NEO_BABYLON);
        case 16:
            return WarpInfo(6, 3, THEME_NEO_BABYLON);
        case 17:
            return WarpInfo(6, 4, THEME_TIAMAT);
        case 18:
            checkLevelContainsMotherStatue();
            return WarpInfo(7, 1, THEME_SUNKEN_CITY);
        case 19:
        {
            auto theme = THEME_SUNKEN_CITY;
            if (!mNextLevelIsEggplantWorld)
            {
                checkLevelContainsMotherStatue();
            }
            else
            {
                if (mGoToEggplantWorld)
                {
                    theme = THEME_EGGPLANT_WORLD;
                }
                mNextLevelIsEggplantWorld = false;
            }
            return WarpInfo(7, 2, theme);
        }
        case 20:
        {
            auto theme = THEME_SUNKEN_CITY;
            if (mNextLevelIsEggplantWorld && mGoToEggplantWorld)
            {
                theme = THEME_EGGPLANT_WORLD;
            }
            return WarpInfo(7, 3, theme);
        }
        case 21:
            return WarpInfo(7, 4, THEME_HUNDUN);
        default:
            DEBUG("Invalid depth level specified");
            return WarpInfo(1, 1, THEME_DWELLING);
        }
    }

    void SeedFinder::checkLevelContainsMotherStatue()
    {
        if (mEggplantMotherStatueHandID == 0)
        {
            mEggplantMotherStatueHandID = to_id("ENT_TYPE_DECORATION_MOTHER_STATUE_HAND");
        }

        auto state = State::get();
        for (const auto item : state.layer(0)->items())
        {
            if (item->type->id == mEggplantMotherStatueHandID)
            {
                mNextLevelIsEggplantWorld = true;
            }
        }
        mNextLevelIsEggplantWorld = false;
    }

    std::vector<Entity*> SeedFinder::entitiesInRect(const Rect& rect, const std::vector<Entity*>& items)
    {
        std::vector<Entity*> retVec;
        for (Entity* item : items)
        {
            if (rect.containsPoint(item->x, item->y))
            {
                retVec.emplace_back(item);
            }
        }
        return retVec;
    }

    const char* SeedFinder::entityName(uint16_t id)
    {
        auto result = std::find_if(mAllEntities.begin(), mAllEntities.end(), [&id](const EntityItem& e) { return e.id == id; });
        if (result != std::end(mAllEntities))
        {
            return result->name.c_str();
        }
        return "(unknown entity)";
    }

    void SeedFinder::handleState()
    {
        switch (mSeedFinderState)
        {
        case SeedFinderState::IDLE:
            // noop
            break;
        case SeedFinderState::PAUSE:
            if (mSeedFinderPauseCounter == 0)
            {
                mSeedFinderState = mSeedFinderNextState;
            }
            else
            {
                mSeedFinderPauseCounter--;
            }
            break;
        case SeedFinderState::INIT:
            initializeNewRun();
            break;
        case SeedFinderState::EXECUTE_LOAD_SEED:
            executeLoadSeed();
            break;
        case SeedFinderState::EXECUTE_FILTERS:
            executeFilters();
            break;
        case SeedFinderState::CLEANUP:
            cleanUp();
            break;
        }
    }

    void SeedFinder::pause(uint16_t frameCount, SeedFinderState nextState)
    {
        mSeedFinderState = SeedFinderState::PAUSE;
        mSeedFinderNextState = nextState;
        mSeedFinderPauseCounter = frameCount;
    }

    void SeedFinder::initializeNewRun()
    {
        mFoundSeeds.clear();
        Util::startLogging();

        mTimeStart = std::chrono::steady_clock::now();

        mCurrentSeed = msStartSeed;
        if (mUseRandomSeeds)
        {
            mCurrentSeed = nextSeed(0);
        }
        mAmountOfSeedsProcessed = 0;

        Util::log("-------------------------------------------------------------------------------------------------------");
        Util::log(fmt::format("Starting seed finder: {} iteration(s)", mAmountToProcess));
        mDeepestLevel = deepestLevelOfAllFilters();
        Util::log(fmt::format("- Deepest level of all filters: {}", mDeepestLevel));

        std::vector<std::string> chosenPath;
        if (mDeepestLevel > gThresholdChooseJungleVolcana)
        {
            chosenPath.emplace_back(mGoToJungle ? "Jungle" : "Volcana");
        }
        if (mDeepestLevel > gThresholdChooseTidePoolTemple)
        {
            if (mGoToTidePool)
            {
                chosenPath.emplace_back("Tide Pool");
                if (mGoToAbzu)
                {
                    chosenPath.emplace_back("Abzu");
                }
            }
            else
            {
                chosenPath.emplace_back("Temple");
                if (mGoToCityOfGold)
                {
                    chosenPath.emplace_back("City of Gold");
                }
                if (mGoToDuat)
                {
                    chosenPath.emplace_back("Duat");
                }
            }
            if (mGoToEggplantWorld)
            {
                chosenPath.emplace_back("Eggplant World");
            }
        }
        if (!chosenPath.empty())
        {
            Util::log(fmt::format("- Chosen path: {}", Util::joinVectorOfStrings(chosenPath, " - ")));
        }

        for (const auto& filter : mFilters)
        {
            filter->writeToLog();
        }
        mSeedFinderState = SeedFinderState::EXECUTE_LOAD_SEED;
    }

    void SeedFinder::cleanUp()
    {
        std::chrono::steady_clock::time_point timeEnd = std::chrono::steady_clock::now();

        if (!mFoundSeeds.empty())
        {
            setGameSeed(mFoundSeeds.at(0));
        }
        else
        {
            setGameSeed(0);
        }

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - mTimeStart).count();
        Util::log(fmt::format("Searching took {} milliseconds = {:.2f} seeds/second", duration, (double)mAmountToProcess / ((double)duration / 1000.)));
        Util::log(fmt::format("Amount of seed(s) found: {}", mFoundSeeds.size()));
        if (mFoundSeeds.size() > 0)
        {
            std::stringstream ss;
            ss << "Seeds:\n\t";
            for (auto x = 0; x < mFoundSeeds.size(); ++x)
            {
                ss << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << mFoundSeeds.at(x) << " ";
                if ((x + 1) % 10 == 0)
                {
                    ss << "\n\t";
                }
            }
            Util::log(ss.str());
        }

        Util::stopLogging();
        mSeedFinderState = SeedFinderState::IDLE;
    }

    void SeedFinder::executeLoadSeed()
    {
        Util::msLogStream.flush();
        Util::log(fmt::format("- Testing seed {:08X}", mCurrentSeed));
        setGameSeed(mCurrentSeed);
        for (const auto& filter : mFilters)
        {
            filter->resetForNewSeed(mCurrentSeed);
        }
        mSeedFinderState = SeedFinderState::EXECUTE_FILTERS;
    }

    void SeedFinder::executeFilters()
    {
        auto satisfiesFilters = true;
        for (auto depth = 0; depth <= mDeepestLevel; ++depth)
        {
            auto warpInfo = warpInfoForDepth(depth);
            warp(warpInfo.world, warpInfo.level, warpInfo.theme);

            for (const auto& filter : mFilters)
            {
                if (filter->shouldExecute(warpInfo.world, warpInfo.level))
                {
                    auto success = filter->execute(warpInfo.world, warpInfo.level);
                    if (!success)
                    {
                        satisfiesFilters = false;
                        break;
                    }
                }
            }
        }
        if (satisfiesFilters)
        {
            Util::log(fmt::format("********************************* SEED {:08X} SATISFIES ALL FILTERS *********************************", mCurrentSeed));
            mFoundSeeds.emplace_back(mCurrentSeed);
        }

        mAmountOfSeedsProcessed++;
        mCurrentSeed = nextSeed(mCurrentSeed);

        if (mAmountOfSeedsProcessed == mAmountToProcess)
        {
            mSeedFinderState = SeedFinderState::CLEANUP;
        }
        else
        {
            if (mAntiFlickerFrameCount > 0)
            {
                pause(mAntiFlickerFrameCount, SeedFinderState::EXECUTE_LOAD_SEED);
            }
            else
            {
                mSeedFinderState = SeedFinderState::EXECUTE_LOAD_SEED;
            }
        }
    }

    bool SeedFinder::areWeGoingToVolcana()
    {
        return !mGoToJungle;
    }

    bool SeedFinder::areWeGoingToJungle()
    {
        return mGoToJungle;
    }

    bool SeedFinder::areWeGoingToTemple()
    {
        return !mGoToTidePool;
    }

    bool SeedFinder::areWeGoingToTidePool()
    {
        return mGoToTidePool;
    }

#ifdef SEEDFINDER_WARPTEST
    std::string SeedFinder::calculateLevelSHA256(uint32_t seed, uint8_t world, uint8_t level, uint8_t theme)
    {
        auto state = State::get();
        warp(world, level, theme);

        auto front = state.layer(0)->items();
        auto back = state.layer(1)->items();

        SHA256 sha;

        std::stringstream ss;
        ss << mCurrentSeed << static_cast<int>(world) << static_cast<int>(level) << static_cast<int>(theme);

        for (Entity* entity : front)
        {
            ss << entity->type->id << entity->uid << entity->x << entity->y;
        }
        for (Entity* entity : back)
        {
            ss << entity->type->id << entity->uid << entity->x << entity->y;
        }

        sha.update(ss.str());
        return SHA256::toString(sha.digest());
    }

    void SeedFinder::warpTest()
    {
        uint32_t seed = 0xCAFEBABE;
        setGameSeed(seed);
        DEBUG("straight");
        DEBUG("{:08X} {}-{} DWELLING = {}", seed, 1, 1, calculateLevelSHA256(seed, 1, 1, THEME_DWELLING));
        DEBUG("{:08X} {}-{} DWELLING = {}", seed, 1, 2, calculateLevelSHA256(seed, 1, 2, THEME_DWELLING));
        DEBUG("{:08X} {}-{} DWELLING = {}", seed, 1, 3, calculateLevelSHA256(seed, 1, 3, THEME_DWELLING));
        DEBUG("{:08X} {}-{} DWELLING = {}", seed, 1, 4, calculateLevelSHA256(seed, 1, 4, THEME_DWELLING));
        DEBUG("{:08X} {}-{} JUNGLE   = {}", seed, 2, 1, calculateLevelSHA256(seed, 2, 1, THEME_JUNGLE));
        DEBUG("{:08X} {}-{} JUNGLE   = {}", seed, 2, 2, calculateLevelSHA256(seed, 2, 2, THEME_JUNGLE));
        DEBUG("{:08X} {}-{} JUNGLE   = {}", seed, 2, 3, calculateLevelSHA256(seed, 2, 3, THEME_JUNGLE));
        DEBUG("{:08X} {}-{} JUNGLE   = {}", seed, 2, 4, calculateLevelSHA256(seed, 2, 4, THEME_JUNGLE));
        DEBUG("{:08X} {}-{} OLMEC    = {}", seed, 3, 1, calculateLevelSHA256(seed, 3, 1, THEME_OLMEC));
        DEBUG("{:08X} {}-{} TIDEPOOL = {}", seed, 4, 1, calculateLevelSHA256(seed, 4, 1, THEME_TIDE_POOL));
        DEBUG("{:08X} {}-{} TIDEPOOL = {}", seed, 4, 2, calculateLevelSHA256(seed, 4, 2, THEME_TIDE_POOL));
        DEBUG("{:08X} {}-{} TIDEPOOL = {}", seed, 4, 3, calculateLevelSHA256(seed, 4, 3, THEME_TIDE_POOL));
        DEBUG("{:08X} {}-{} TIDEPOOL = {}", seed, 4, 4, calculateLevelSHA256(seed, 4, 4, THEME_TIDE_POOL));
        DEBUG("{:08X} {}-{} ICECAVES = {}", seed, 5, 1, calculateLevelSHA256(seed, 5, 1, THEME_ICE_CAVES));
        DEBUG("{:08X} {}-{} NEOBAB   = {}", seed, 6, 1, calculateLevelSHA256(seed, 6, 1, THEME_NEO_BABYLON));
        DEBUG("{:08X} {}-{} NEOBAB   = {}", seed, 6, 2, calculateLevelSHA256(seed, 6, 2, THEME_NEO_BABYLON));
        DEBUG("{:08X} {}-{} NEOBAB   = {}", seed, 6, 3, calculateLevelSHA256(seed, 6, 3, THEME_NEO_BABYLON));
        DEBUG("{:08X} {}-{} NEOBAB   = {}", seed, 6, 4, calculateLevelSHA256(seed, 6, 4, THEME_NEO_BABYLON));

        setGameSeed(seed);
        DEBUG("1-1 doubled");
        DEBUG("{:08X} {}-{} DWELLING = {}", seed, 1, 1, calculateLevelSHA256(seed, 1, 1, THEME_DWELLING));
        DEBUG("{:08X} {}-{} DWELLING = {}", seed, 1, 1, calculateLevelSHA256(seed, 1, 1, THEME_DWELLING));
        DEBUG("{:08X} {}-{} DWELLING = {}", seed, 1, 2, calculateLevelSHA256(seed, 1, 2, THEME_DWELLING));
        DEBUG("{:08X} {}-{} DWELLING = {}", seed, 1, 3, calculateLevelSHA256(seed, 1, 3, THEME_DWELLING));
        DEBUG("{:08X} {}-{} DWELLING = {}", seed, 1, 4, calculateLevelSHA256(seed, 1, 4, THEME_DWELLING));
        DEBUG("{:08X} {}-{} JUNGLE   = {}", seed, 2, 1, calculateLevelSHA256(seed, 2, 1, THEME_JUNGLE));
        DEBUG("{:08X} {}-{} JUNGLE   = {}", seed, 2, 2, calculateLevelSHA256(seed, 2, 2, THEME_JUNGLE));
        DEBUG("{:08X} {}-{} JUNGLE   = {}", seed, 2, 3, calculateLevelSHA256(seed, 2, 3, THEME_JUNGLE));
        DEBUG("{:08X} {}-{} JUNGLE   = {}", seed, 2, 4, calculateLevelSHA256(seed, 2, 4, THEME_JUNGLE));
        DEBUG("{:08X} {}-{} OLMEC    = {}", seed, 3, 1, calculateLevelSHA256(seed, 3, 1, THEME_OLMEC));
        DEBUG("{:08X} {}-{} TIDEPOOL = {}", seed, 4, 1, calculateLevelSHA256(seed, 4, 1, THEME_TIDE_POOL));
        DEBUG("{:08X} {}-{} TIDEPOOL = {}", seed, 4, 2, calculateLevelSHA256(seed, 4, 2, THEME_TIDE_POOL));
        DEBUG("{:08X} {}-{} TIDEPOOL = {}", seed, 4, 3, calculateLevelSHA256(seed, 4, 3, THEME_TIDE_POOL));
        DEBUG("{:08X} {}-{} TIDEPOOL = {}", seed, 4, 4, calculateLevelSHA256(seed, 4, 4, THEME_TIDE_POOL));
        DEBUG("{:08X} {}-{} ICECAVES = {}", seed, 5, 1, calculateLevelSHA256(seed, 5, 1, THEME_ICE_CAVES));
        DEBUG("{:08X} {}-{} NEOBAB   = {}", seed, 6, 1, calculateLevelSHA256(seed, 6, 1, THEME_NEO_BABYLON));
        DEBUG("{:08X} {}-{} NEOBAB   = {}", seed, 6, 2, calculateLevelSHA256(seed, 6, 2, THEME_NEO_BABYLON));
        DEBUG("{:08X} {}-{} NEOBAB   = {}", seed, 6, 3, calculateLevelSHA256(seed, 6, 3, THEME_NEO_BABYLON));
        DEBUG("{:08X} {}-{} NEOBAB   = {}", seed, 6, 4, calculateLevelSHA256(seed, 6, 4, THEME_NEO_BABYLON));

        setGameSeed(seed);
        DEBUG("straight to 2-1 jungle");
        DEBUG("{:08X} {}-{} JUNGLE   = {}", seed, 2, 1, calculateLevelSHA256(seed, 2, 1, THEME_JUNGLE));

        setGameSeed(seed);
        DEBUG("straight to 3-1 olmec");
        DEBUG("{:08X} {}-{} OLMEC    = {}", seed, 3, 1, calculateLevelSHA256(seed, 3, 1, THEME_OLMEC));

        setGameSeed(seed);
        DEBUG("straight to 4-1 tide pool");
        DEBUG("{:08X} {}-{} TIDEPOOL = {}", seed, 4, 1, calculateLevelSHA256(seed, 4, 1, THEME_TIDE_POOL));

        setGameSeed(seed);
        DEBUG("straight to 5-1 ice caves");
        DEBUG("{:08X} {}-{} ICECAVES = {}", seed, 5, 1, calculateLevelSHA256(seed, 5, 1, THEME_ICE_CAVES));

        setGameSeed(seed);
        DEBUG("straight to 6-1 neo babylon");
        DEBUG("{:08X} {}-{} NEOBAB   = {}", seed, 6, 1, calculateLevelSHA256(seed, 6, 1, THEME_NEO_BABYLON));

        setGameSeed(seed);
        DEBUG("straight, but alternate themes");
        DEBUG("{:08X} {}-{} DWELLING = {}", seed, 1, 1, calculateLevelSHA256(seed, 1, 1, THEME_DWELLING));
        DEBUG("{:08X} {}-{} DWELLING = {}", seed, 1, 2, calculateLevelSHA256(seed, 1, 2, THEME_DWELLING));
        DEBUG("{:08X} {}-{} DWELLING = {}", seed, 1, 3, calculateLevelSHA256(seed, 1, 3, THEME_DWELLING));
        DEBUG("{:08X} {}-{} DWELLING = {}", seed, 1, 4, calculateLevelSHA256(seed, 1, 4, THEME_DWELLING));
        DEBUG("{:08X} {}-{} VOLCANA  = {}", seed, 2, 1, calculateLevelSHA256(seed, 2, 1, THEME_VOLCANA));
        DEBUG("{:08X} {}-{} VOLCANA  = {}", seed, 2, 2, calculateLevelSHA256(seed, 2, 2, THEME_VOLCANA));
        DEBUG("{:08X} {}-{} VOLCANA  = {}", seed, 2, 3, calculateLevelSHA256(seed, 2, 3, THEME_VOLCANA));
        DEBUG("{:08X} {}-{} VOLCANA  = {}", seed, 2, 4, calculateLevelSHA256(seed, 2, 4, THEME_VOLCANA));
        DEBUG("{:08X} {}-{} OLMEC    = {}", seed, 3, 1, calculateLevelSHA256(seed, 3, 1, THEME_OLMEC));
        DEBUG("{:08X} {}-{} TEMPLE   = {}", seed, 4, 1, calculateLevelSHA256(seed, 4, 1, THEME_TEMPLE));
        DEBUG("{:08X} {}-{} TEMPLE   = {}", seed, 4, 2, calculateLevelSHA256(seed, 4, 2, THEME_TEMPLE));
        DEBUG("{:08X} {}-{} TEMPLE   = {}", seed, 4, 3, calculateLevelSHA256(seed, 4, 3, THEME_TEMPLE));
        DEBUG("{:08X} {}-{} TEMPLE   = {}", seed, 4, 4, calculateLevelSHA256(seed, 4, 4, THEME_TEMPLE));
        DEBUG("{:08X} {}-{} ICECAVES = {}", seed, 5, 1, calculateLevelSHA256(seed, 5, 1, THEME_ICE_CAVES));
        DEBUG("{:08X} {}-{} NEOBAB   = {}", seed, 6, 1, calculateLevelSHA256(seed, 6, 1, THEME_NEO_BABYLON));
        DEBUG("{:08X} {}-{} NEOBAB   = {}", seed, 6, 2, calculateLevelSHA256(seed, 6, 2, THEME_NEO_BABYLON));
        DEBUG("{:08X} {}-{} NEOBAB   = {}", seed, 6, 3, calculateLevelSHA256(seed, 6, 3, THEME_NEO_BABYLON));
        DEBUG("{:08X} {}-{} NEOBAB   = {}", seed, 6, 4, calculateLevelSHA256(seed, 6, 4, THEME_NEO_BABYLON));
    }
#endif

} // namespace SeedFinder