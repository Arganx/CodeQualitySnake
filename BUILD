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

cc_test(
    name = "limited_test",
    srcs = ["test/limited_uint8_8_test.cpp"],
    copts = CPP_FLAGS_DEBUG,
    deps = [
        ":limited",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "board_position_test",
    srcs = ["test/board_position_test.cpp"],
    copts = CPP_FLAGS_DEBUG,
    deps = [
        ":board_position",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "board_test",
    srcs = [
        "test/board_test.cpp",
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
        "test/snake_test.cpp",
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
        "test/game_test.cpp",
    ],
    copts = CPP_FLAGS_DEBUG,
    defines = ["TESTING"],
    deps = [
        ":game_testing",
        "@googletest//:gtest_main",
    ],
)
