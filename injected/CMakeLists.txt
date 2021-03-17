add_library(Spelunky2SeedFinderInjected SHARED "")

target_include_directories(Spelunky2SeedFinderInjected PRIVATE include)
target_include_directories(Spelunky2SeedFinderInjected PRIVATE ../3rdParty/overlunky/src/game_api)
target_include_directories(Spelunky2SeedFinderInjected PRIVATE ../3rdParty/overlunky/src/imgui)
target_include_directories(Spelunky2SeedFinderInjected PRIVATE ../3rdParty/overlunky/src/shared/)
target_include_directories(Spelunky2SeedFinderInjected PRIVATE ../3rdParty/overlunky/src/fmt/include/)
target_include_directories(Spelunky2SeedFinderInjected PUBLIC ../3rdParty/json/single_include/)

target_link_libraries(Spelunky2SeedFinderInjected fmt Shlwapi)

add_subdirectory(src)

set_target_properties(Spelunky2SeedFinderInjected PROPERTIES OUTPUT_NAME Spelunky2SeedFinder)

if (MSVC)
        target_compile_options(Spelunky2SeedFinderInjected PRIVATE /bigobj)
endif()
