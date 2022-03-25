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
    const char* SeedFinder::kJSONVersion = "version";
    const char* SeedFinder::kJSONConfiguration = "configuration";
    const char* SeedFinder::kJSONAmountOfSeeds = "amount_of_seeds";
    const char* SeedFinder::kJSONUseRandomSeed = "use_random_seed";
    const char* SeedFinder::kJSONStartSeed = "start_seed";
    const char* SeedFinder::kJSONAntiFlickerFramecount = "anti_flicker_framecount";
    const char* SeedFinder::kJSONGoToJungle = "go_to_jungle";
    const char* SeedFinder::kJSONGoToTidePool = "go_to_tidepool";
    const char* SeedFinder::kJSONGoToCityOfGold = "go_to_city_of_gold";
    const char* SeedFinder::kJSONGoToAbzu = "go_to_abzu";
    const char* SeedFinder::kJSONGoToDuat = "go_to_duat";
    const char* SeedFinder::kJSONGoToEggplantWorld = "go_to_eggplant_world";
    const char* SeedFinder::kJSONFilters = "filters";
    const char* SeedFinder::kJSONFilterID = "id";
    const char* SeedFinder::kJSONAccessibility = "accessibility";
    const char* SeedFinder::kJSONLevels = "levels";
    const char* SeedFinder::kJSONLayer = "layer";
    const char* SeedFinder::kJSONItem = "item";
    const char* SeedFinder::kJSONMinimum = "minimum";

    const char* SeedFinder::msComboWarpOptions[cWarpOptionsCount] = {"1-1", "1-2", "1-3",       "Quillback", "2-1", "2-2", "2-3",    "2-4", "Olmec", "4-1", "4-2",
                                                                     "4-3", "4-4", "Ice caves", "6-1",       "6-2", "6-3", "Tiamat", "7-1", "7-2",   "7-3", "Hundun"};

    SeedFinder::SeedFinder()
    {
        mFilterTitles = std::map<std::string, std::string, bool (*)(const std::string&, const std::string&)>(
            [](const std::string& a, const std::string& b) -> bool { return (_stricmp(a.c_str(), b.c_str()) < 0); });
        registerFilters();
    }

    void SeedFinder::registerAllEntities(const std::vector<EntityItem>& entities)
    {
        g_state = State::get().ptr();
        mAllEntities = entities;
        std::sort(mAllEntities.begin(), mAllEntities.end(), [](const EntityItem& a, const EntityItem& b) -> bool { return (a.name.compare(b.name) < 0); });
    }

    void SeedFinder::registerFilters()
    {
        registerFilter(FilterFindAltar::uniqueIdentifier(), FilterFindAltar::title(), &FilterFindAltar::instantiate);
        registerFilter(FilterFindAnkhSkip::uniqueIdentifier(), FilterFindAnkhSkip::title(), &FilterFindAnkhSkip::instantiate);
        registerFilter(FilterFindBlackMarket::uniqueIdentifier(), FilterFindBlackMarket::title(), &FilterFindBlackMarket::instantiate);
        registerFilter(FilterFindCavemanShopWithSpecificContents::uniqueIdentifier(), FilterFindCavemanShopWithSpecificContents::title(), &FilterFindCavemanShopWithSpecificContents::instantiate);
        registerFilter(FilterFindCrateWithSpecificContents::uniqueIdentifier(), FilterFindCrateWithSpecificContents::title(), &FilterFindCrateWithSpecificContents::instantiate);
        registerFilter(FilterFindDiceShop::uniqueIdentifier(), FilterFindDiceShop::title(), &FilterFindDiceShop::instantiate);
        registerFilter(FilterFindEntity::uniqueIdentifier(), FilterFindEntity::title(), &FilterFindEntity::instantiate);
        registerFilter(FilterFindGhostJar::uniqueIdentifier(), FilterFindGhostJar::title(), &FilterFindGhostJar::instantiate);
        registerFilter(FilterFindItemInCrust::uniqueIdentifier(), FilterFindItemInCrust::title(), &FilterFindItemInCrust::instantiate);
        registerFilter(FilterFindLevelFeelings::uniqueIdentifier(), FilterFindLevelFeelings::title(), &FilterFindLevelFeelings::instantiate);
        registerFilter(FilterFindMoonChallenge::uniqueIdentifier(), FilterFindMoonChallenge::title(), &FilterFindMoonChallenge::instantiate);
        registerFilter(FilterFindPet::uniqueIdentifier(), FilterFindPet::title(), &FilterFindPet::instantiate);
        registerFilter(FilterFindPotWithSpecificContents::uniqueIdentifier(), FilterFindPotWithSpecificContents::title(), &FilterFindPotWithSpecificContents::instantiate);
        registerFilter(FilterFindQuillbackObstruction::uniqueIdentifier(), FilterFindQuillbackObstruction::title(), &FilterFindQuillbackObstruction::instantiate);
        registerFilter(FilterFindRegularShopWithSpecificContents::uniqueIdentifier(), FilterFindRegularShopWithSpecificContents::title(), &FilterFindRegularShopWithSpecificContents::instantiate);
        registerFilter(FilterFindSisters::uniqueIdentifier(), FilterFindSisters::title(), &FilterFindSisters::instantiate);
        registerFilter(FilterFindSunChallenge::uniqueIdentifier(), FilterFindSunChallenge::title(), &FilterFindSunChallenge::instantiate);
        registerFilter(FilterFindTunShopWithSpecificContents::uniqueIdentifier(), FilterFindTunShopWithSpecificContents::title(), &FilterFindTunShopWithSpecificContents::instantiate);
        registerFilter(FilterFindUdjatEye::uniqueIdentifier(), FilterFindUdjatEye::title(), &FilterFindUdjatEye::instantiate);
        registerFilter(FilterFindVanHorsing::uniqueIdentifier(), FilterFindVanHorsing::title(), &FilterFindVanHorsing::instantiate);
        registerFilter(FilterFindVault::uniqueIdentifier(), FilterFindVault::title(), &FilterFindVault::instantiate);
        registerFilter(FilterFindVolcanaDrill::uniqueIdentifier(), FilterFindVolcanaDrill::title(), &FilterFindVolcanaDrill::instantiate);
    }

    const std::vector<EntityItem>& SeedFinder::allEntities()
    {
        return mAllEntities;
    }

    void SeedFinder::registerFilter(const std::string& uniqueIdentifier, const std::string& filterTitle, std::function<std::unique_ptr<Filter>(SeedFinder*)> instantiator)
    {
        mFilterInstantiators[uniqueIdentifier] = instantiator;
        mFilterTitles[filterTitle] = uniqueIdentifier;
    }

    Filter* SeedFinder::createFilterForIdentifier(const std::string& filterIdentifier)
    {
        if (mFilterInstantiators.count(filterIdentifier) > 0)
        {
            auto instance = mFilterInstantiators[filterIdentifier](this);
            Filter* returnPtr = instance.get();
            mFilters.emplace_back(std::move(instance));
            return returnPtr;
        }
        return nullptr;
    }

    uint32_t SeedFinder::currentSeed() const noexcept
    {
        return mCurrentSeed;
    }

    void SeedFinder::render()
    {
        if (mSeedFinderState != SeedFinderState::IDLE || (g_state->ingame && g_state->playing))
        {
            switch (mInterfaceState)
            {
            case InterfaceState::MAIN:
                renderMainInterface();
                break;
            case InterfaceState::IMPORT:
                renderImport();
                break;
            case InterfaceState::EXPORT:
                renderExport();
                break;
            }
        }
        else
        {
            const char* caption = "To search for seeds, please make sure you are in an actual level";
            ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x / 2.) - (ImGui::CalcTextSize(caption).x / 2.), ImGui::GetCursorPosY() + 40));
            ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), caption);

            const char* caption2 = "Press F11 to hide/show this window";
            ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x / 2.) - (ImGui::CalcTextSize(caption2).x / 2.), ImGui::GetCursorPosY() + 40));
            ImGui::Text(caption2);
        }
    }

    void SeedFinder::renderMainInterface()
    {
        static const float buttonWidth = 80;
        bool allFiltersValid = areAllFiltersValid();
        // ************* CONFIGURATION *************
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Text("Configuration");

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowSize().x - buttonWidth - 10);
        if (ImGui::Button("Import##SeedFinderImportButton", ImVec2(buttonWidth, 0)))
        {
            mInterfaceState = InterfaceState::IMPORT;
        }
        if (allFiltersValid)
        {
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetWindowSize().x - (buttonWidth * 2) - 15);
            if (ImGui::Button("Export##SeedFinderExportButton", ImVec2(buttonWidth, 0)))
            {
                mInterfaceState = InterfaceState::EXPORT;
            }
        }

        ImGui::SetCursorPosX(gSectionMarginHor);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Amount of seeds to process :");
        ImGui::SameLine();
        ImGui::PushItemWidth(100);
        ImGui::InputScalar("##SeedFinderAmountOfSeeds", ImGuiDataType_U32, &mAmountToProcess);

        ImGui::SetCursorPosX(gSectionMarginHor);
        ImGui::Checkbox("Generate random seeds, or start at seed:##SeedFinderGenerateRandomSeeds", &mUseRandomSeeds);
        ImGui::SameLine();
        ImGui::InputScalar("##SeedFinderStartAtSeed", ImGuiDataType_U32, &mStartSeed, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal | (mUseRandomSeeds ? ImGuiInputTextFlags_ReadOnly : 0));

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
            for (const auto& kv : mFilterTitles)
            {
                bool isSelected = (mComboAddFilterChosenTitle == kv.first.c_str());
                if (ImGui::Selectable(kv.first.c_str(), isSelected))
                {
                    mComboAddFilterChosenTitle = kv.first.c_str();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::SameLine();
        if (ImGui::Button("Add filter##SeedFinderAddFilter"))
        {
            if (mFilterTitles.count(mComboAddFilterChosenTitle) > 0)
            {
                createFilterForIdentifier(mFilterTitles.at(mComboAddFilterChosenTitle));
            }
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
                ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x / 2.) - (buttonWidth / 2.), ImGui::GetCursorPosY() + 10));
                if (ImGui::Button("Find Seeds##SeedFinderFindSeeds", ImVec2(buttonWidth, 0)))
                {
                    pause(30, SeedFinderState::INIT);
                }
            }
            else
            {
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

                    if (mComboWarpChosenDepth > 0)
                    {
                        mSafeWarpFrameCounter = cSafeWarpFrames;
                        mSafeWarpCurrentDepth = 0;
                        mSafeWarpMaxDepth = mComboWarpChosenDepth;
                        mSeedFinderState = SeedFinderState::SAFE_WARP;
                    }
                }
            }
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

            auto state = State::get();
            ImGui::SetCursorPosX(gSectionMarginHor);
            if (ImGui::Checkbox("God mode##SeedFinderGodMode", &mGodMode))
            {
                state.godmode(mGodMode);
            }

            ImGui::SameLine();
            if (ImGui::Checkbox("Zoom out##SeedFinderZoomOut", &mZoomOut))
            {
                if (mZoomOut)
                {
                    state.zoom(29.87);
                }
                else
                {
                    state.zoom(13.5);
                }
            }

            ImGui::SetCursorPosX(gSectionMarginHor);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Warp to :");
            ImGui::SameLine();
            ImGui::PushItemWidth(120);
            if (ImGui::BeginCombo(fmt::format("##SeedFinderWarpToLevelCombo{}", fmt::ptr(this)).c_str(), mComboWarpChosenLevel))
            {
                for (auto i = 0; i < cWarpOptionsCount; ++i)
                {
                    bool isSelected = (mComboWarpChosenLevel == msComboWarpOptions[i]);
                    if (ImGui::Selectable(msComboWarpOptions[i], isSelected))
                    {
                        mComboWarpChosenLevel = msComboWarpOptions[i];
                        mComboWarpChosenDepth = i;
                    }
                }
                ImGui::EndCombo();
            }
        }

        handleState();

        // clean up deleted filters
        mFilters.erase(std::remove_if(mFilters.begin(), mFilters.end(), [](const std::unique_ptr<Filter>& f) { return f->isDeleted(); }), mFilters.end());
    }

    void SeedFinder::renderImport()
    {
        ImGui::Text("Import");

        ImGui::SetCursorPosX(gSectionMarginHor);
        ImGui::Text("Please paste the json information in the field below");

        ImGui::SetCursorPosX(gSectionMarginHor);
        constexpr size_t bufferSize = 1024 * 500;
        static char buffer[bufferSize] = {0};
        if (ImGui::InputTextMultiline("##SeedFinderImportJSON", buffer, bufferSize, {ImGui::GetWindowSize().x - (gSectionMarginHor * 2), ImGui::GetWindowSize().y - ImGui::GetCursorPosY() - 60}))
        {
            mUnserializationError = "";
        }

        if (!mUnserializationError.empty())
        {
            ImGui::SetCursorPosX(gSectionMarginHor);
            ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), mUnserializationError.c_str());
        }

        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::Button("Import##SeedFinderPerformImport"))
        {
            try
            {
                auto j = json::parse(buffer);
                unserialize(j);
            }
            catch (const json::exception& e)
            {
                mUnserializationError = e.what();
            }
            catch (...)
            {
                mUnserializationError = "Error parsing the provided JSON data";
            }
            if (mUnserializationError.empty())
            {
                mInterfaceState = InterfaceState::MAIN;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel##SeedFinderCancelImport"))
        {
            mInterfaceState = InterfaceState::MAIN;
        }
    }

    void SeedFinder::renderExport()
    {
        ImGui::Text("Export");

        ImGui::SetCursorPosX(gSectionMarginHor);
        ImGui::Text("Please copy the json information below and store it somewhere");

        ImGui::SetCursorPosX(gSectionMarginHor);
        auto j = serialize();
        auto jsonString = j.dump(4);
        ImGui::InputTextMultiline("##SeedFinderExportJSON", const_cast<char*>(jsonString.c_str()), jsonString.size(),
                                  {ImGui::GetWindowSize().x - (gSectionMarginHor * 2), ImGui::GetWindowSize().y - ImGui::GetCursorPosY() - 50},
                                  ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);

        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::Button("Back##SeedFinderBackFromExportToMain"))
        {
            mInterfaceState = InterfaceState::MAIN;
        }
    }

    json SeedFinder::serialize()
    {
        json j;
        j[kJSONVersion] = 1;
        j[kJSONConfiguration][kJSONAmountOfSeeds] = mAmountToProcess;
        if (mUseRandomSeeds)
        {
            j[kJSONConfiguration][kJSONUseRandomSeed] = mUseRandomSeeds;
        }
        else
        {
            j[kJSONConfiguration][kJSONStartSeed] = mStartSeed;
        }
        j[kJSONConfiguration][kJSONAntiFlickerFramecount] = mAntiFlickerFrameCount;
        j[kJSONConfiguration][kJSONGoToJungle] = mGoToJungle;
        j[kJSONConfiguration][kJSONGoToTidePool] = mGoToTidePool;
        j[kJSONConfiguration][kJSONGoToCityOfGold] = mGoToCityOfGold;
        j[kJSONConfiguration][kJSONGoToAbzu] = mGoToAbzu;
        j[kJSONConfiguration][kJSONGoToDuat] = mGoToDuat;
        j[kJSONConfiguration][kJSONGoToEggplantWorld] = mGoToEggplantWorld;

        for (const auto& filter : mFilters)
        {
            j[kJSONFilters].push_back(filter->serialize());
        }

        return j;
    }

    void SeedFinder::unserialize(const json& o)
    {
        mFilters.clear();
        mAmountToProcess = 10;
        mUseRandomSeeds = true;
        mStartSeed = 0;
        mAntiFlickerFrameCount = 30;
        mGoToJungle = true;
        mGoToTidePool = true;
        mGoToCityOfGold = false;
        mGoToAbzu = false;
        mGoToDuat = false;
        mGoToEggplantWorld = false;

        try
        {
            if (o.contains(kJSONVersion))
            {
                auto version = o.at(kJSONVersion).get<uint8_t>();
                if (version == 1)
                {
                    if (o.contains(kJSONConfiguration))
                    {
                        auto config = o.at(kJSONConfiguration);
                        if (config.contains(kJSONAmountOfSeeds))
                        {
                            mAmountToProcess = config.at(kJSONAmountOfSeeds).get<uint32_t>();
                        }
                        if (config.contains(kJSONUseRandomSeed))
                        {
                            mUseRandomSeeds = config.at(kJSONUseRandomSeed).get<bool>();
                        }
                        if (config.contains(kJSONStartSeed))
                        {
                            mStartSeed = config.at(kJSONStartSeed).get<uint32_t>();
                        }
                        if (config.contains(kJSONAntiFlickerFramecount))
                        {
                            mAntiFlickerFrameCount = config.at(kJSONAntiFlickerFramecount).get<uint32_t>();
                        }
                        if (config.contains(kJSONGoToJungle))
                        {
                            mGoToJungle = config.at(kJSONGoToJungle).get<bool>();
                        }
                        if (config.contains(kJSONGoToTidePool))
                        {
                            mGoToTidePool = config.at(kJSONGoToTidePool).get<bool>();
                        }
                        if (config.contains(kJSONGoToCityOfGold))
                        {
                            mGoToCityOfGold = config.at(kJSONGoToCityOfGold).get<bool>();
                        }
                        if (config.contains(kJSONGoToAbzu))
                        {
                            mGoToAbzu = config.at(kJSONGoToAbzu).get<bool>();
                        }
                        if (config.contains(kJSONGoToDuat))
                        {
                            mGoToDuat = config.at(kJSONGoToDuat).get<bool>();
                        }
                        if (config.contains(kJSONGoToEggplantWorld))
                        {
                            mGoToEggplantWorld = config.at(kJSONGoToEggplantWorld).get<bool>();
                        }
                    }
                    if (o.contains(kJSONFilters))
                    {
                        for (const auto& filterEntry : o.at(kJSONFilters))
                        {
                            if (filterEntry.contains(kJSONFilterID))
                            {
                                auto filterID = filterEntry.at(kJSONFilterID).get<std::string>();
                                auto filter = createFilterForIdentifier(filterID);
                                if (filter != nullptr)
                                {
                                    auto result = filter->unserialize(filterEntry);
                                    if (!result.empty())
                                    {
                                        mUnserializationError = result;
                                        return;
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    mUnserializationError = "Version mismatch, can't read this version";
                }
            }
        }
        catch (const json::exception& e)
        {
            mUnserializationError = e.what();
        }
        catch (...)
        {
            mUnserializationError = "Exception occurred while parsing the provided JSON data";
        }
    }

    void SeedFinder::setGameSeed(uint32_t seed)
    {
        if (g_state != nullptr)
        {
            ::set_seed(seed);
        }
    };

    void SeedFinder::warp(uint8_t world, uint8_t level, uint8_t theme)
    {
        if (g_state != nullptr)
        {
            ::warp(world, level, theme);
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
        for (const auto item : state.layer(0)->all_entities.entities())
        {
            if (item->type->id == mEggplantMotherStatueHandID)
            {
                mNextLevelIsEggplantWorld = true;
            }
        }
        mNextLevelIsEggplantWorld = false;
    }

    std::vector<Entity*> SeedFinder::entitiesInRect(const Rect& rect, const EntityList::EntityRange& items)
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
        case SeedFinderState::SAFE_WARP:
            --mSafeWarpFrameCounter;
            if (mSafeWarpFrameCounter == 0)
            {
                auto warpInfo = warpInfoForDepth(mSafeWarpCurrentDepth);
                g_state->world_next = warpInfo.world;
                g_state->level_next = warpInfo.level;
                g_state->theme_next = warpInfo.theme;
                g_state->loading = 1;

                mSafeWarpCurrentDepth++;
                mSafeWarpFrameCounter = cSafeWarpFrames;
                if (mSafeWarpCurrentDepth > mSafeWarpMaxDepth)
                {
                    mSeedFinderState = SeedFinderState::IDLE;
                }
            }
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

        mCurrentSeed = mStartSeed;
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

        auto front = state.layer(0)->all_entities.entities();
        auto back = state.layer(1)->all_entities.entities();

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