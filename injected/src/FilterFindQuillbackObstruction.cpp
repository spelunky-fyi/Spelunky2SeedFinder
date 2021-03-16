#include "FilterFindQuillbackObstruction.h"
#include "AStar/SimplifiedLevel.h"
#include "SeedFinder.h"

namespace SeedFinder
{
    uint16_t FilterFindQuillbackObstruction::msExitDoorID = 0;
    uint16_t FilterFindQuillbackObstruction::msLayerDoorID = 0;
    const char* FilterFindQuillbackObstruction::msExistances[] = {"must not", "must"};

    FilterFindQuillbackObstruction::FilterFindQuillbackObstruction(SeedFinder* seedFinder) : Filter(seedFinder), mCheckJungleSide(true), mCheckVolcanaSide(true), mExistance(msExistances[0])
    {
        if (msExitDoorID == 0)
        {
            msExitDoorID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_EXIT"));
            msLayerDoorID = static_cast<uint16_t>(to_id("ENT_TYPE_FLOOR_DOOR_LAYER"));
        }
    }

    std::string FilterFindQuillbackObstruction::title() { return "Find Quillback obstruction"; }

    uint8_t FilterFindQuillbackObstruction::deepestLevel() const
    {
        return 3; // 1-4
    }

    bool FilterFindQuillbackObstruction::shouldExecute(uint8_t currentWorld, uint8_t currentLevel) { return (currentWorld == 1 && currentLevel == 4); }

    bool FilterFindQuillbackObstruction::isValid()
    {
        setLastError("");
        if (!mCheckJungleSide && !mCheckVolcanaSide)
        {
            setLastError("Choose jungle or volcana side, or both");
        }
        return (mCheckJungleSide || mCheckVolcanaSide);
    }

    bool FilterFindQuillbackObstruction::execute(uint8_t currentWorld, uint8_t currentLevel)
    {
        auto state = State::get();
        auto frontLayerEntities = state.layer(0)->items();
        auto simpleLevel = SimplifiedLevel(mSeedFinder, state.ptr()->w, state.ptr()->h, frontLayerEntities, true);

        const auto findDoor = [frontLayerEntities](const Rect& rect, uint16_t doorID) -> std::pair<uint8_t, uint8_t> {
            for (Entity* entity : frontLayerEntities)
            {
                auto x = static_cast<uint8_t>(std::floor(entity->x));
                auto y = static_cast<uint8_t>(std::floor(entity->y));
                if (entity->type->id == doorID && rect.containsPoint(x, y))
                {
                    return std::make_pair(x, y);
                }
            }
            return std::make_pair(0, 0);
        };

        bool jungleSideSatisfied = (mCheckJungleSide == false ? true : false);
        bool volcanaSideSatisfied = (mCheckVolcanaSide == false ? true : false);

        if (mCheckJungleSide)
        {
            auto [jungleLayerDoorX, jungleLayerDoorY] = findDoor(Rect(1, 92, 6, 6), msLayerDoorID);
            auto [jungleExitDoorX, jungleExitDoorY] = findDoor(Rect(1, 83, 10, 7), msExitDoorID);
            if (jungleLayerDoorY == 0 || jungleExitDoorY == 0)
            {
                Util::log(fmt::format("- FilterFindQuillbackObstruction: jungle side door could not be found. This is a bug! layer: {}, {} exit {}, {}", jungleLayerDoorX, jungleLayerDoorY,
                                      jungleExitDoorX, jungleExitDoorY));
                return false;
            }
            Util::log(
                fmt::format("- FilterFindQuillbackObstruction: found jungle side layer door at {}, {} and exit door at {}, {}", jungleLayerDoorX, jungleLayerDoorY, jungleExitDoorX, jungleExitDoorY));
            auto connected = simpleLevel.areTilesConnected(jungleLayerDoorX, jungleLayerDoorY, jungleExitDoorX, jungleExitDoorY);
            Util::log(fmt::format("- FilterFindQuillbackObstruction: jungle side doors are connected: {}", connected));

            if (mExistance == msExistances[0])
            {
                jungleSideSatisfied = (connected == true);
            }
            else
            {
                jungleSideSatisfied = (connected == false);
            }
        }

        if (mCheckVolcanaSide)
        {
            auto [volcanaLayerDoorX, volcanaLayerDoorY] = findDoor(Rect(47, 92, 6, 6), msLayerDoorID);
            auto [volcanaExitDoorX, volcanaExitDoorY] = findDoor(Rect(43, 83, 10, 7), msExitDoorID);
            if (volcanaLayerDoorY == 0 || volcanaExitDoorY == 0)
            {
                Util::log(fmt::format("- FilterFindQuillbackObstruction: volcana side door could not be found. This is a bug! layer: {}, {} exit {}, {}", volcanaLayerDoorX, volcanaLayerDoorY,
                                      volcanaExitDoorX, volcanaExitDoorY));
                return false;
            }
            Util::log(fmt::format("- FilterFindQuillbackObstruction: found volcana side layer door at {}, {} and exit door at {}, {}", volcanaLayerDoorX, volcanaLayerDoorY, volcanaExitDoorX,
                                  volcanaExitDoorY));
            auto connected = simpleLevel.areTilesConnected(volcanaLayerDoorX, volcanaLayerDoorY, volcanaExitDoorX, volcanaExitDoorY);
            Util::log(fmt::format("- FilterFindQuillbackObstruction: volcana side doors are connected: {}", connected));

            if (mExistance == msExistances[0])
            {
                volcanaSideSatisfied = (connected == true);
            }
            else
            {
                volcanaSideSatisfied = (connected == false);
            }
        }

        return (jungleSideSatisfied && volcanaSideSatisfied);
    }

    void FilterFindQuillbackObstruction::render()
    {
        ImGui::SetCursorPosX(gSectionMarginHor);
        if (ImGui::CollapsingHeader(fmt::format("{}##SeedFinderFilterFindQuillbackObstructionHeader{}", title(), fmt::ptr(this)).c_str()), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::SetCursorPosX(gSectionMarginHor * 2);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Side to check: ");
            ImGui::SameLine();

            ImGui::Checkbox(fmt::format("Jungle##SeedFinderFilterFindQuillbackObstructionSideJungle{}", fmt::ptr(this)).c_str(), &mCheckJungleSide);
            ImGui::SameLine();
            ImGui::Checkbox(fmt::format("Volcana##SeedFinderFilterFindQuillbackObstructionSideVolcana{}", fmt::ptr(this)).c_str(), &mCheckVolcanaSide);

            FilterUI::renderDeleteFilterButton(this);

            ImGui::SetCursorPosX(gSectionMarginHor * 2);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Obstruction");
            ImGui::SameLine();
            ImGui::PushItemWidth(100);
            if (ImGui::BeginCombo(fmt::format("##SeedFinderFilterFindQuillbackObstructionMustNotExist{}", fmt::ptr(this)).c_str(), mExistance))
            {
                for (auto i = 0; i < IM_ARRAYSIZE(msExistances); ++i)
                {
                    bool isSelected = (mExistance == msExistances[i]);
                    if (ImGui::Selectable(msExistances[i], isSelected))
                    {
                        mExistance = msExistances[i];
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("exist");
        }
    }

    void FilterFindQuillbackObstruction::writeToLog()
    {
        Util::log(fmt::format("- Filter: FilterFindQuillbackObstruction"));
        Util::log(fmt::format("\tCheck Jungle side: {} | Check Volcana side: {}", mCheckJungleSide, mCheckVolcanaSide));
        Util::log(fmt::format("\tObstruction {} exist", mExistance));
    }
} // namespace SeedFinder