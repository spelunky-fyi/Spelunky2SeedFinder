file(GLOB_RECURSE Spelunky2SeedFinderInjected_Sources CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE Spelunky2SeedFinderInjected_Headers CONFIGURE_DEPENDS include/*.h include/*.hpp include/*.inl)

add_library(Spelunky2SeedFinderInjected SHARED ${Spelunky2SeedFinderInjected_Sources} ${Spelunky2SeedFinderInjected_Headers})

target_include_directories(Spelunky2SeedFinderInjected PRIVATE include)
target_include_directories(Spelunky2SeedFinderInjected PUBLIC ../3rdParty/json/single_include/)

target_link_libraries(Spelunky2SeedFinderInjected fmt Shlwapi spel2_api shared)

set_target_properties(Spelunky2SeedFinderInjected PROPERTIES OUTPUT_NAME Spelunky2SeedFinder)

if (MSVC)
        target_compile_options(Spelunky2SeedFinderInjected PRIVATE /bigobj)
endif()
