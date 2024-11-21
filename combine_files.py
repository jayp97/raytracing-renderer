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

    with open("combined.txt", "w") as outfile:
        for file in src_dir.glob("*.cpp"):
            with open(file, "r") as infile:
                outfile.write(infile.read() + "\n")

        for file in include_dir.glob("*.h"):
            with open(file, "r") as infile:
                outfile.write(infile.read() + "\n")

        for file in scenes_dir.glob("*.json"):
            with open(file, "r") as infile:
                outfile.write(infile.read() + "\n")


# Example usage:
root_directory = ""
combine_files(root_directory)
