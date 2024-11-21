import os
from pathlib import Path


def combine_files(root_dir):
    """Combines files from 'src', 'include', and 'scenes' folders into a single .txt file.

    Args:
      root_dir: The root directory containing the 'src', 'include', and 'scenes' folders.
    """

    src_dir = Path(root_dir) / "src"
    include_dir = Path(root_dir) / "include"
    scenes_dir = Path(root_dir) / "scenes"
    tests_dir = Path(root_dir) / "test"

    with open("combined.txt", "w") as outfile:
        for file in src_dir.glob("*.cpp"):
            with open(file, "r") as infile:
                outfile.write(infile.read() + "\n")

        for file in include_dir.glob("*.h"):
            with open(file, "r") as infile:
                outfile.write(infile.read() + "\n")

        for file in scenes_dir.glob("*.json"):
            with open(file, "r") as infile:
                # add file name at top of each scene before adding the content
                outfile.write(file.name + "\n" + infile.read() + "\n")
        # add the Makefile to the combined file
        with open("Makefile", "r") as infile:
            outfile.write(infile.read() + "\n")
            ## And the test_raytracer.cpp file in the tests folder
        with open(tests_dir / "test_raytracer.cpp", "r") as infile:
            outfile.write(infile.read() + "\n")


# Example usage:
root_directory = ""
combine_files(root_directory)
