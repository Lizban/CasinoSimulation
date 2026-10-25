cmake_minimum_required(VERSION 3.24)
project(Casino LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)

include(FetchContent)
FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG 52eb8108c5bdec04579160ae17225d66034bd723)
FetchContent_MakeAvailable(googletest)

option(COVERAGE "Build with coverage" OFF)
if(COVERAGE)
  add_compile_options(--coverage)
  add_link_options(--coverage)
endif()

file(GLOB_RECURSE CASINO_SOURCES CONFIGURE_DEPENDS src/*.cpp)
list(REMOVE_ITEM CASINO_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)

add_library(casino_lib ${CASINO_SOURCES})
target_compile_features(casino_lib PUBLIC cxx_std_17)
target_include_directories(casino_lib PUBLIC src)

if(EXISTS ${CMAKE_SOURCE_DIR}/src/main.cpp)
  add_executable(casino src/main.cpp)
  target_link_libraries(casino PRIVATE casino_lib)
endif()

add_executable(casino_tests ${TEST_SOURCES})
target_link_libraries(casino_tests PRIVATE casino_lib GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(casino_tests DISCOVERY_MODE PRE_TEST)
