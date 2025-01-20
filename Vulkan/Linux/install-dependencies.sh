#!/bin/bash

echo -e "\033[1mInstalling dependencies...\n\n\033[0m"

sudo apt install vulkan-tools libvulkan-dev vulkan-validationlayers-dev spirv-tools libglfw3-dev libglm-dev

vulkaninfo
echo -e "\033[1m** If you don't see Vulkan information or the cube, your system doesn't support Vulkan **\n\033[0m"
vkcube
