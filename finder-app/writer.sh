#!/bin/bash

# Check if both arguments were specified
if [ -z "$1" ] || [ -z "$2" ]; then
  echo "Error: Please specify writefile and writestr"
  exit 1
fi

# Create the directory path if it doesn't exist
mkdir -p "$(dirname "$1")"

# Write the content to the file
echo "$2" > "$1"

# Check if the file was created successfully
if [ ! -f "$1" ]; then
  echo "Error: $1 could not be created"
  exit 1
fi
