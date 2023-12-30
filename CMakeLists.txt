cmake_minimum_required(VERSION 3.22.0)
project(CMakeSetup VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

file(GLOB SOURCES "src/*.cpp")
include_directories(inc)

enable_testing()

add_executable(
    limited_test
    test/board_position_test.cpp
    ${SOURCES}
)

add_executable(
    board_position_test
    test/board_position_test.cpp
    ${SOURCES}
)

add_executable(
    board_test
    test/board_test.cpp
    ${SOURCES}
)

add_executable(
    game_test
    test/game_test.cpp
    ${SOURCES}
)

add_executable(
    snake_test
    test/snake_test.cpp
    ${SOURCES}
)

target_link_libraries(
    limited_test
    GTest::gtest_main
)

target_link_libraries(
    board_position_test
    GTest::gtest_main
)

target_link_libraries(
    board_test
    GTest::gtest_main
)

target_link_libraries(
    game_test
    GTest::gtest_main
)

target_link_libraries(
    snake_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(limited_test)
gtest_discover_tests(board_test)
gtest_discover_tests(game_test)
gtest_discover_tests(board_position_test)
gtest_discover_tests(snake_test)
target_compile_definitions(game_test PRIVATE TESTING)

# Tools config
include_directories("tools/inc")
file(GLOB TOOLS "tools/src/*.cpp")

add_executable(
    texture_loader_test
    tools/tests/texture_loader_test.cpp
    tools/src/texture_loader.cpp
)

target_link_libraries(
    texture_loader_test
    GTest::gtest_main
    sfml-graphics sfml-window sfml-system
)

gtest_discover_tests(texture_loader_test)

add_executable(
    visualiser_test
    tools/tests/visualiser_test.cpp
    tools/src/visualiser.cpp
    ${SOURCES}
)

target_link_libraries(
    visualiser_test
    GTest::gtest_main
    sfml-graphics sfml-window sfml-system
)

gtest_discover_tests(visualiser_test)

add_executable(
    snake_game
    main.cpp
    tools/src/visualiser.cpp
    ${SOURCES}
    ${TOOLS}
)

target_link_libraries(
    snake_game
    sfml-graphics sfml-window sfml-system
)
