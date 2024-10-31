# Set the compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Itest

# Directories for source, test, and output files
SRC_DIR = src
TEST_DIR = test
OBJ_DIR = obj
BIN_DIR = bin

# List of source and test files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/raytracer
TEST_TARGET = $(BIN_DIR)/test_ray

# Default target to build the main executable
all: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Target to build and run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Link object files to create the test executable
$(TEST_TARGET): $(OBJECTS) $(TEST_OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each .cpp file in src to an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile each .cpp file in test to an object file
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure output directories exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Clean command to remove generated files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
