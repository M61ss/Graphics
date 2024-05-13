# Preparing the workspace

## GLFW

**GLFW** is a library written in C that provides **abstraction** to save us all operating-system level operations needed to display windows on different machines which could adopt different operating systems and may have different hardware specifications.

## Setting up

There is a very well written guide at [this site](https://learnopengl.com/Getting-started/Creating-a-window) that explain step by step how to install necessary softwares, setup the workspace, download and compile external liberaries.

## Why I need to compile myself binaries?

The problem with providing source code to the open world however is that not everyone uses the same IDE or build system for developing their application, which means the project/solution files provided may not be compatible with other people's setup. So people then have to setup their own project/solution with the given .c/.cpp and .h/.hpp files, which is cumbersome. Exactly for those reasons there is a tool called **CMake**.
