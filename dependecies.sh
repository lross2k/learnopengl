#! /usr/bin/bash
# Install missing dependecies on debian almost base install
sudo apt-get install libxrandr-dev libxinerama-dev libxcursor-dev # Also could install xorg-dev
# Make a directory for GLFW build and build it with CMake and GNU Make
cd glfw-source-package
mkdir build && cd build
cmake ..
make -j4 # I currently have a quadcore

