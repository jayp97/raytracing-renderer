CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# List all source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/raytracer

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
    $(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
    $(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
    mkdir -p $@

clean:
    rm -rf $(OBJ_DIR) $(BIN_DIR)
