
 

#!/bin/bash

# Check if both arguments were specified
if [ -z "$1" ] || [ -z "$2" ]; then
  echo "Error: Please specify filesdir and searchstr"
  exit 1
fi

# Check if filesdir is a directory on the filesystem
if [ ! -d "$1" ]; then
  echo "Error: $1 is not a directory on the filesystem"
  exit 1
fi

# Count the number of files and matching lines
num_files=$(find "$1" -type f | wc -l)
num_lines=$(grep -r "$2" "$1" | wc -l)

# Print the results
echo "The number of files are $num_files and the number of matching lines are $num_lines"
