compiler = g++
flags = -g -std=c++20 -O0 -Wextra -pedantic -Wall -Wconversion -pthread --coverage


# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file, or if you want to use
# a copy of Google Test at a different location.
GTEST_DIR = ../googletest/googletest

# Points to the root of Google Mock, relative to where this file is.
# Remember to tweak this if you move this file.
GMOCK_DIR = ../googletest/googlemock

# Where to find user code.
USER_DIR = ../tests

# Where to find tools code
TOOLS_DIR = ../tools
TOOLS_TESTS = $(TOOLS_DIR)/tests
TOOLS_INC = $(TOOLS_DIR)/inc
TOOLS_SRC = $(TOOLS_DIR)/src
CONTROLLERS_INC = $(TOOLS_DIR)/SFML-tools/inc
CONTROLLERS_SRC =$(TOOLS_DIR)/SFML-tools/src

# Flags passed to the preprocessor.
# Set Google Test and Google Mock's header directories as system
# directories, such that the compiler doesn't generate warnings in
# these headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
# All Google Mock headers. Note that all Google Test headers are
# included here too, as they are #included by Google Mock headers.
# Usually you shouldn't change this definition.
GMOCK_HEADERS = $(GMOCK_DIR)/include/gmock/*.h \
                $(GMOCK_DIR)/include/gmock/internal/*.h \
                $(GTEST_HEADERS)

INC_DIR = ../inc
SRC_DIR = ../src



# main: ../main.cpp
# 	$(compiler) $(flags) -I$(INC_DIR) ../main.cpp $(SRC_DIR)/* -o main

clean:
	rm -rf build/*

# Builds gmock.a and gmock_main.a.  These libraries contain both
# Google Mock and Google Test.  A test should link with either gmock.a
# or gmock_main.a, depending on whether it defines its own main()
# function.  It's fine if your test only uses features from Google
# Test (and not Google Mock).
# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
GMOCK_SRCS_ = $(GMOCK_DIR)/src/*.cc $(GMOCK_HEADERS)


# For simplicity and to avoid depending on implementation details of
# Google Mock and Google Test, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Mock and
# Google Test compile fast and for ordinary users their source rarely
# changes.

TESTING_FLAG = -D TESTING

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(flags) \
            -c $(GTEST_DIR)/src/gtest-all.cc
gmock-all.o : $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(flags) \
            -c $(GMOCK_DIR)/src/gmock-all.cc
gmock_main.o : $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(flags) \
            -c $(GMOCK_DIR)/src/gmock_main.cc
gmock.a : gmock-all.o gtest-all.o
	$(AR) $(ARFLAGS) $@ $^
gmock_main.a : gmock-all.o gtest-all.o gmock_main.o
	$(AR) $(ARFLAGS) $@ $^


game_test.o : $(USER_DIR)/game_test.cpp $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) -I$(INC_DIR) $(flags) -c $(USER_DIR)/game_test.cpp
game_test : game_test.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(TESTING_FLAG) $(flags) -lpthread $^ $(SRC_DIR)/* -o $@

board_test.o : $(USER_DIR)/board_test.cpp $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) -I$(INC_DIR) $(flags) -c $(USER_DIR)/board_test.cpp
board_test : board_test.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(TESTING_FLAG) $(flags) -lpthread $^ $(SRC_DIR)/board_position.cpp $(SRC_DIR)/limited_uint8_t.cpp $(SRC_DIR)/board.cpp -o $@

board_position_test.o : $(USER_DIR)/board_position_test.cpp $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) -I$(INC_DIR) $(flags) -c $(USER_DIR)/board_position_test.cpp
board_position_test : board_position_test.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(TESTING_FLAG) $(flags) -lpthread $^ $(SRC_DIR)/board_position.cpp $(SRC_DIR)/limited_uint8_t.cpp -o $@

snake_test.o : $(USER_DIR)/snake_test.cpp $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) -I$(INC_DIR) $(flags) -c $(USER_DIR)/snake_test.cpp
snake_test : snake_test.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(TESTING_FLAG) $(flags) -lpthread $^ $(SRC_DIR)/board_position.cpp $(SRC_DIR)/limited_uint8_t.cpp $(SRC_DIR)/segment.cpp $(SRC_DIR)/snake.cpp -o $@

limited_uint8_8_test.o : $(USER_DIR)/limited_uint8_8_test.cpp $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) -I$(INC_DIR) $(flags) -c $(USER_DIR)/limited_uint8_8_test.cpp
limited_uint8_8_test : limited_uint8_8_test.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(TESTING_FLAG) $(flags) -lpthread $^ $(SRC_DIR)/limited_uint8_t.cpp -o $@

visualiser_test.o : $(TOOLS_TESTS)/visualiser_test.cpp $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) -I$(INC_DIR) -I$(TOOLS_INC) $(flags) -c $(TOOLS_TESTS)/visualiser_test.cpp
visualiser_test : visualiser_test.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(TESTING_FLAG) $(flags) -lpthread $^ $(SRC_DIR)/* $(TOOLS_SRC)/visualiser.cpp -o $@

texture_loader_test.o : $(TOOLS_TESTS)/texture_loader_test.cpp $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) -I$(INC_DIR) -I$(TOOLS_INC) $(flags) -c $(TOOLS_TESTS)/texture_loader_test.cpp
texture_loader_test : texture_loader_test.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(TESTING_FLAG) $(flags) -lpthread $^ $(SRC_DIR)/* $(TOOLS_SRC)/texture_loader.cpp -o $@ -lsfml-graphics

drawer_test.o : $(TOOLS_TESTS)/drawer_test.cpp $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) -I$(INC_DIR) -I$(TOOLS_INC) $(flags) -c $(TOOLS_TESTS)/drawer_test.cpp
drawer_test : drawer_test.o gmock_main.a
	$(CXX) $(CPPFLAGS) $(TESTING_FLAG) $(flags) -lpthread $^ $(SRC_DIR)/* $(TOOLS_SRC)/drawer.cpp -o $@ -lsfml-graphics -lsfml-window -lsfml-system

tests: board_test game_test board_position_test snake_test limited_uint8_8_test visualiser_test texture_loader_test drawer_test

SFML_INC = /home/argan/libs/SFML-2.6.1/include
SFML_LIB = /home/argan/libs/SFML-2.6.1/lib

main: ../main.cpp
	$(compiler) $(flags) -I$(INC_DIR) -I$(CONTROLLERS_INC) ../main.cpp $(SRC_DIR)/* $(TOOLS_SRC)/* $(CONTROLLERS_SRC)/* -o main -lsfml-graphics -lsfml-window -lsfml-system

ALL = main tests
all : $(ALL)

menu: ../menu_example.cpp
	g++ $(flags) -I../tools/inc/* -I../tools/SFML-tools/inc/* ../menu_example.cpp ../tools/SFML-tools/src/menu.cpp ../tools/src/screen_selector.cpp -o menu -lsfml-graphics -lsfml-window -lsfml-system