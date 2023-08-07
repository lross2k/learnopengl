#! /usr/bin/bash

glfw_version='3.3.8'

# Install missing dependecies on debian almost base install
sudo apt-get install libxrandr-dev libxinerama-dev libxcursor-dev # Also could install xorg-dev

# GLFW stuff
wget -nc https://github.com/glfw/glfw/releases/download/${glfw_version}/glfw-${glfw_version}.zip
unzip -n glfw-${glfw_version}.zip
cd "glfw-${glfw_version}"
mkdir -p build
cd build
cmake ..
make -j$(nproc)
cp src/libglfw3.a ../../Libs/
mkdir -p ../../Include/GLFW
cp ../include/GLFW/glfw3.h ../../Include/GLFW/
cp ../include/GLFW/glfw3native.h ../../Include/GLFW/
cd ../..
