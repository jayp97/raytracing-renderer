# Set the compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Directories for source and output files
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# List of source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/raytracer

# Default target to build the executable
all: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each .cpp file to an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure output directories exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Clean command to remove generated files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
