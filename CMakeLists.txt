cmake_minimum_required(VERSION 3.17)
project(Spelunky2SeedFinder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

execute_process(
  COMMAND git describe --always --dirty=-modified
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions("-DGIT_VERSION=${GIT_VERSION}")
add_definitions("-D_ITERATOR_DEBUG_LEVEL=0")

set(FMT_MASTER_PROJECT OFF)
add_subdirectory(3rdParty/overlunky/src/fmt)

add_subdirectory(injector)
add_subdirectory(injected)

if (MSVC)
  add_definitions(/bigobj)
endif()
