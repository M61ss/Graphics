#!/bin/bash

echo "Installing dependencies...\n\n"

sudo apt install vulkan-tools libvulkan-dev vulkan-validationlayers-dev spirv-tools libglfw3-dev libglm-dev

vulkaninfo
vkcube

echo "If you don't see Vulkan information or the cube, your system doesn't support Vulkan.\n"
