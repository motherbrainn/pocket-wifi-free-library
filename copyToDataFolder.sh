#!/bin/bash

# Hardcoded source directory
source_dir="data"

# Check if the destination file exists
if [ ! -f "destination.txt" ]; then
    echo "Destination file 'destination.txt' not found."
    exit 1
fi

# Read the destination directory from the file
destination_dir=$(cat "destination.txt")

# Check if the source directory exists
if [ ! -d "$source_dir" ]; then
    echo "Source directory does not exist: $source_dir"
    exit 1
fi

# Check if the destination directory exists, or create it if it doesn't
if [ ! -d "$destination_dir" ]; then
    mkdir -p "$destination_dir"
fi

# Copy all files from the source directory to the destination directory
cp -r "$source_dir"/* "$destination_dir"

echo "Files copied from $source_dir to $destination_dir"
