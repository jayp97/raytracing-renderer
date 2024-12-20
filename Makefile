# Set the compiler and flags
CXX = /opt/homebrew/bin/g++-14
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -Wpedantic -fopenmp \
           -Iinclude -Itest \
           -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.1.sdk
LDFLAGS = -L/opt/homebrew/lib/gcc/14

# Directories for source, test, and output files
SRC_DIR = src
TEST_DIR = test
OBJ_DIR = obj
BIN_DIR = bin

# List of source files and object files (excluding main.cpp for tests)
SOURCES = $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
MAIN_OBJECT = $(OBJ_DIR)/main.o

# List of test sources and objects
TEST_SOURCES = $(filter-out $(SRC_DIR)/main.cpp, $(SOURCES)) \
               $(TEST_DIR)/test_camera.cpp $(TEST_DIR)/test_sphere.cpp $(TEST_DIR)/test_ray.cpp \
               $(TEST_DIR)/catch_amalgamated.cpp
TEST_OBJECTS = $(TEST_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Executable targets
TARGET = $(BIN_DIR)/raytracer
TEST_TARGET_CAMERA = $(BIN_DIR)/test_camera
TEST_TARGET_SPHERE = $(BIN_DIR)/test_sphere
TEST_TARGET_RAY = $(BIN_DIR)/test_ray
TEST_TARGET_TRIANGLE = $(BIN_DIR)/test_triangle
TEST_TARGET_CYLINDER = $(BIN_DIR)/test_cylinder
TEST_TARGET_SHADOW = $(BIN_DIR)/test_shadow
TEST_TARGET_SCENE_LOADER = $(BIN_DIR)/test_scene_loader
TEST_TARGET_RAYTRACER = $(BIN_DIR)/test_raytracer
TEST_TARGET_BINARY_RENDERING_MODE = $(BIN_DIR)/test_binary_rendering_mode

# Default target to build the main executable
all: $(TARGET)

# Link object files to create the main executable
$(TARGET): $(OBJECTS) $(MAIN_OBJECT) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Build and run the test_camera executable
test_camera: $(OBJECTS) $(OBJ_DIR)/test_camera.o $(OBJ_DIR)/catch_amalgamated.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET_CAMERA) $(OBJECTS) $(OBJ_DIR)/test_camera.o $(OBJ_DIR)/catch_amalgamated.o $(LDFLAGS)
	./$(TEST_TARGET_CAMERA)

# Build and run the test_sphere executable
test_sphere: $(OBJECTS) $(OBJ_DIR)/test_sphere.o $(OBJ_DIR)/catch_amalgamated.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET_SPHERE) $(OBJECTS) $(OBJ_DIR)/test_sphere.o $(OBJ_DIR)/catch_amalgamated.o $(LDFLAGS)
	./$(TEST_TARGET_SPHERE)

# Build and run the test_ray executable
test_ray: $(OBJECTS) $(OBJ_DIR)/test_ray.o $(OBJ_DIR)/catch_amalgamated.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET_RAY) $(OBJECTS) $(OBJ_DIR)/test_ray.o $(OBJ_DIR)/catch_amalgamated.o $(LDFLAGS)
	./$(TEST_TARGET_RAY)

# Compile and run the test_triangle executable
test_triangle: $(OBJECTS) $(OBJ_DIR)/test_triangle.o $(OBJ_DIR)/catch_amalgamated.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET_TRIANGLE) $(OBJECTS) $(OBJ_DIR)/test_triangle.o $(OBJ_DIR)/catch_amalgamated.o $(LDFLAGS)
	./$(TEST_TARGET_TRIANGLE)

# Compile and run the test_cylinder executable
test_cylinder: $(OBJECTS) $(OBJ_DIR)/test_cylinder.o $(OBJ_DIR)/catch_amalgamated.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET_CYLINDER) $(OBJECTS) $(OBJ_DIR)/test_cylinder.o $(OBJ_DIR)/catch_amalgamated.o $(LDFLAGS)
	./$(TEST_TARGET_CYLINDER)

# Compile and run the test_shadow executable
test_shadow: $(OBJECTS) $(OBJ_DIR)/test_shadow.o $(OBJ_DIR)/catch_amalgamated.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET_SHADOW) $(OBJECTS) $(OBJ_DIR)/test_shadow.o $(OBJ_DIR)/catch_amalgamated.o $(LDFLAGS)
	./$(TEST_TARGET_SHADOW)

# Rule for compiling and running the test_scene_loader executable
test_scene_loader: $(OBJECTS) $(OBJ_DIR)/test_scene_loader.o $(OBJ_DIR)/catch_amalgamated.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET_SCENE_LOADER) $(OBJECTS) $(OBJ_DIR)/test_scene_loader.o $(OBJ_DIR)/catch_amalgamated.o $(LDFLAGS)
	./$(TEST_TARGET_SCENE_LOADER)

# Compile and run the test_raytracer executable
test_raytracer: $(OBJECTS) $(OBJ_DIR)/test_raytracer.o $(OBJ_DIR)/catch_amalgamated.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET_RAYTRACER) $(OBJECTS) $(OBJ_DIR)/test_raytracer.o $(OBJ_DIR)/catch_amalgamated.o $(LDFLAGS)
	./$(TEST_TARGET_RAYTRACER)

# Compile and run the test_binary_rendering_mode executable
test_binary_rendering_mode: $(OBJECTS) $(OBJ_DIR)/test_binary_rendering_mode.o $(OBJ_DIR)/catch_amalgamated.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET_BINARY_RENDERING_MODE) $(OBJECTS) $(OBJ_DIR)/test_binary_rendering_mode.o $(OBJ_DIR)/catch_amalgamated.o $(LDFLAGS)
	./$(TEST_TARGET_BINARY_RENDERING_MODE)

# Compile each .cpp file in src to an object file (including main.cpp)
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
	rm -rf $(OBJ_DIR) $(BIN_DIR) *.o *.gch
