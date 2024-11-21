#!/bin/bash

# Ensure the script is executable
# chmod +x render_scenes.sh

# Array of input JSON and output PPM file pairs
declare -a scenes=(
    "binary_primitives.json binary_primitives.ppm"
    "simple_phong.json simple_phong.ppm"
    "mirror_image.json mirror_image.ppm"
    "scene.json scene_binary.ppm"
    "scene_phong.json scene_phong.ppm" # Ensure this JSON has rendermode set to "phong"
)

# Path to the raytracer executable
RAYTRACER_EXEC="./bin/raytracer"

# Loop through each scene and render
for scene_pair in "${scenes[@]}"; do
    read -r inputJson outputPpm <<< "$scene_pair"
    echo "Rendering $inputJson to $outputPpm..."
    $RAYTRACER_EXEC "$inputJson" "$outputPpm"
done

echo "All scenes have been rendered."
