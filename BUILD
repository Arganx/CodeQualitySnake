load("//:variables.bzl", "CPP_FLAGS_DEBUG")

cc_library(
    name = "limited",
    srcs = ["src/limited_uint8_t.cpp"],
    hdrs = ["inc/limited_uint8_t.hpp"],
    copts = CPP_FLAGS_DEBUG,
)

cc_library(
    name = "board_position",
    srcs = ["src/board_position.cpp"],
    hdrs = ["inc/board_position.hpp"],
    copts = CPP_FLAGS_DEBUG,
    deps = [":limited"],
)

cc_library(
    name = "segment",
    srcs = ["src/segment.cpp"],
    hdrs = ["inc/segment.hpp"],
    copts = CPP_FLAGS_DEBUG,
    deps = [":board_position"],
)

cc_library(
    name = "board",
    srcs = ["src/board.cpp"],
    hdrs = [
        "inc/board.hpp",
        "inc/board_mappings.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    deps = [":board_position"],
)

cc_library(
    name = "snake",
    srcs = ["src/snake.cpp"],
    hdrs = [
        "inc/exceptions.hpp",
        "inc/snake.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    deps = [":segment"],
)

cc_library(
    name = "game",
    srcs = ["src/game.cpp"],
    hdrs = [
        "inc/direction.hpp",
        "inc/game.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    deps = [
        ":board",
        ":snake",
    ],
)

cc_library(
    name = "game_testing",
    srcs = ["src/game.cpp"],
    hdrs = [
        "inc/direction.hpp",
        "inc/game.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    defines = ["TESTING"],
    deps = [
        ":board",
        ":snake",
    ],
)

cc_library(
    name = "visualiser",
    srcs = ["tools/src/visualiser.cpp"],
    hdrs = [
        "tools/inc/visualiser.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    deps = [
        ":board",
    ],
)

cc_library(
    name = "texture_loader",
    srcs = ["tools/src/texture_loader.cpp"],
    hdrs = [
        "tools/inc/texture_loader.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
)

cc_library(
    name = "text_box",
    srcs = ["tools/src/text_box.cpp"],
    hdrs = [
        "tools/inc/text_box.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
)

cc_library(
    name = "screen_selector",
    srcs = ["tools/src/screen_selector.cpp"],
    hdrs = [
        "tools/inc/screen_selector.hpp",
        "tools/inc/selector_options.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
)

cc_library(
    name = "menu_controller",
    srcs = ["tools/SFML-controllers/src/menu_controller.cpp"],
    hdrs = [
        "tools/SFML-controllers/inc/menu_controller.hpp",
        "tools/inc/config/texture_config.hpp",
        "tools/inc/exceptions.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    deps = [":screen_selector"],
)

cc_library(
    name = "new_game_controller",
    srcs = ["tools/SFML-controllers/src/new_game_controller.cpp"],
    hdrs = [
        "tools/SFML-controllers/inc/new_game_controller.hpp",
        "tools/inc/config/texture_config.hpp",
        "tools/inc/exceptions.hpp",
        "tools/inc/mutexes.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    deps = [
        ":database_manager",
        ":drawer",
        ":game",
        ":options_manager",
        ":screen_selector",
        ":visualiser",
    ],
)

cc_library(
    name = "high_score_controller",
    srcs = ["tools/SFML-controllers/src/high_score_controller.cpp"],
    hdrs = [
        "tools/SFML-controllers/inc/high_score_controller.hpp",
        "tools/inc/config/texture_config.hpp",
        "tools/inc/config/variable_config.hpp",
        "tools/inc/exceptions.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    deps = [
        ":database_manager",
        ":screen_selector",
    ],
)

cc_library(
    name = "options_controller",
    srcs = ["tools/SFML-controllers/src/options_controller.cpp"],
    hdrs = [
        "tools/SFML-controllers/inc/options_controller.hpp",
        "tools/inc/config/texture_config.hpp",
        "tools/inc/exceptions.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    deps = [
        ":options_manager",
        ":screen_selector",
        ":text_box",
    ],
)

cc_library(
    name = "drawer",
    srcs = ["tools/src/drawer.cpp"],
    hdrs = [
        "tools/inc/drawer.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
)

cc_library(
    name = "candies",
    srcs = ["tools/src/candies.cpp"],
    hdrs = [
        "tools/inc/candies.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
)

cc_library(
    name = "assets_manager",
    srcs = ["tools/src/assets_manager.cpp"],
    hdrs = [
        "tools/inc/assets_manager.hpp",
        "tools/inc/exceptions.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    deps = [":texture_loader"],
)

cc_library(
    name = "database_manager",
    srcs = ["tools/src/database_manager.cpp"],
    hdrs = [
        "tools/inc/config/variable_config.hpp",
        "tools/inc/database_manager.hpp",
        "tools/inc/exceptions.hpp",
    ],
    copts = CPP_FLAGS_DEBUG + [
        "-ldl",
        "-lpthread",
    ],
    deps = [
        ":sqlite3",
    ],
)

cc_library(
    name = "options_manager",
    srcs = ["tools/src/options_manager.cpp"],
    hdrs = [
        "tools/inc/options_manager.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    deps = [":candies"],
)

cc_library(
    name = "sqlite3",
    srcs = ["SQL-Lite/sqlite3.c"],
    hdrs = ["SQL-Lite/sqlite3.h"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "limited_test",
    srcs = ["tests/limited_uint8_8_test.cpp"],
    copts = CPP_FLAGS_DEBUG,
    deps = [
        ":limited",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "board_position_test",
    srcs = ["tests/board_position_test.cpp"],
    copts = CPP_FLAGS_DEBUG,
    deps = [
        ":board_position",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "board_test",
    srcs = [
        "tests/board_test.cpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    deps = [
        ":board",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "snake_test",
    srcs = [
        "tests/snake_test.cpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    deps = [
        ":snake",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "game_test",
    srcs = [
        "tests/game_test.cpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    defines = ["TESTING"],
    deps = [
        ":game_testing",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "visualiser_test",
    srcs = ["tools/tests/visualiser_test.cpp"],
    copts = CPP_FLAGS_DEBUG,
    deps = [
        ":visualiser",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "texture_loader_test",
    srcs = ["tools/tests/texture_loader_test.cpp"],
    copts = CPP_FLAGS_DEBUG,
    data = [
        "inc",
        "test-images",
    ],
    env = {"DISPLAY": ":0"},
    linkopts = [
        "-lsfml-graphics",
    ],
    tags = ["local"],
    deps = [
        ":texture_loader",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "drawer_test",
    srcs = ["tools/tests/drawer_test.cpp"],
    copts = CPP_FLAGS_DEBUG,
    env = {"DISPLAY": ":0"},
    linkopts = [
        "-lsfml-graphics",
        "-lsfml-window",
        "-lsfml-system",
    ],
    tags = ["local"],
    deps = [
        ":drawer",
        "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "graphicalSnake",
    srcs = [
        "main.cpp",
        "tools/inc/mutexes.hpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    data = [
        "fonts",
        "textures",
    ],
    linkopts = [
        "-lsfml-graphics",
        "-lsfml-window",
        "-lsfml-system",
    ],
    deps = [
        ":assets_manager",
        ":database_manager",
        ":drawer",
        ":game",
        ":high_score_controller",
        ":menu_controller",
        ":new_game_controller",
        ":options_controller",
        ":options_manager",
        ":visualiser",
    ],
)
