CXX = g++
CXXFLAGS = -g --std=c++20 -fsanitize=address
BUILD_DIR = build
TARGET = $(BUILD_DIR)/main

# Swap this to bench.cpp to see benchmarks
SRCS = src/main.cpp

TEST_TARGET = $(BUILD_DIR)/run_tests
TEST_BUILD_DIR = $(BUILD_DIR)/tests
TEST_SRCS = \
	tests/doctest_main.cpp \
	tests/test_main.cpp

TEST_OBJS = $(patsubst tests/%.cpp,$(TEST_BUILD_DIR)/%.o,$(TEST_SRCS))
TEST_DEPS = $(patsubst tests/%.cpp,$(TEST_BUILD_DIR)/%.d,$(TEST_SRCS))

OBJS = $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS = $(patsubst src/%.cpp,$(BUILD_DIR)/%.d,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: src/%.ipp | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_TARGET): $(TEST_OBJS) $(filter-out $(BUILD_DIR)/main.o $(BUILD_DIR)/bench.o, $(OBJS)) | $(TEST_BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_BUILD_DIR)/%.o: tests/%.cpp | $(TEST_BUILD_DIR)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)

.PHONY: all run clean

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(DEPS)

-include $(DEPS) $(TEST_DEPS)

