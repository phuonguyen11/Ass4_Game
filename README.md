# Assignment 4 - Game UI Setup

This repository contains the files necessary to set up the user interface (UI) for the game.

## Setup Instructions

To set up the game UI, follow these steps:

1. Navigate to the `Assignment4_FE` directory using the command line:
```
  cd Assignment4_FE
```
2. Create a `build` directory inside `Assignment4_FE`:
```
  mkdir build
```
3. Run CMake to generate the necessary build files. Specify the build system, for example, `Win32` for Windows:
```
  cmake -S . -B ./build -A Win32
```
4. Use Visual Studio (preferably Visual Studio 2022) to compile the solution `Assignment4.sln`, located in the `build` directory.
5. Upon successful compilation, `Assignment4.exe` will be generated. Execute this file to launch and enjoy the game!

Running the Server

To run the server:

```
  node server.js
```
