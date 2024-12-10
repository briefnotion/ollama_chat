#!/bin/bash

# Copy the file and rename it to good.good
cp "$2/$1" "$2/$1.good"

# Get the current date in the format YYYYMMDD
current_date=$(date +%Y%m%d)

# Move the original file and rename it with the current date appended
mv "$2/$1" "$2/${1%.*}.$current_date"

# copy current version to destination directory
cp "$1" "$2/"
