CXX = g++
CXXFLAGS = -Wall
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests
BIN_DIR = bin
LDFLAGS = -lboost_program_options -lcryptopp -lpthread

EXEC = srv_calc
COMMANDLINE_OPTIONS = -c data/vcalc.conf -l data/vcalc.log

PROJECT = $(BIN_DIR)/$(EXEC)

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/*.hpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

TESTS = $(wildcard $(TEST_DIR)/*.cpp)
# TEST_TARGETS = $(patsubst $(TEST_DIR)/%.cpp,$(BIN_DIR)/%,$(TESTS))
TEST_TARGETS = $(patsubst $(TEST_DIR)/%.cpp,%,$(TESTS))


.PHONY: all clean clean-test test run

all: $(PROJECT)

# test: $(TEST_TARGETS)
test: $(TEST_TARGETS)
	$(foreach t,$^,./$(BIN_DIR)/$(t);)

run: $(PROJECT)
	./$^ $(COMMANDLINE_OPTIONS)

$(PROJECT): $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@

# $(BIN_DIR)/%_test: $(filter-out $(BUILD_DIR)/main.o,$(OBJECTS))
%_test: $(filter-out $(BUILD_DIR)/main.o,$(OBJECTS))
#	$(CXX) $(CXXFLAGS) -DUNIT_TEST_SRV $(patsubst $(BIN_DIR)/%, $(TEST_DIR)/%.cpp, $@) $^ $(LDFLAGS) -lUnitTest++ -o $@
	$(CXX) $(CXXFLAGS) -DUNIT_TEST_SRV $(TEST_DIR)/$@.cpp $^ $(LDFLAGS) -lUnitTest++ -o $(BIN_DIR)/$@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJECTS): $(HEADERS)

clean:
	rm -f $(BIN_DIR)/* $(BUILD_DIR)/*

clean-test:
	rm -f $(BIN_DIR)/*_test