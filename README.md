# Voidfall
Game Engine

## Setup

### Linux

#### Dependencies
```
git
cmake
libgl1-mesa-dev
x11proto-core-dev
libx11-dev
```

#### Build Instructions
Do the following to build the project for Linux after cloning the repository:
```
$ mkdir build
$ cd build
$ cmake ..
$ make -j8
```

The binary should be located at: `build/Voidfall/Voidfall`.

### Windows

#### Dependencies
Things to make sure you have installed before you can build the project:
* git: [https://git-scm.com/download/win](https://git-scm.com/download/win)
* CMake: [https://cmake.org/download/](https://cmake.org/download/)
* OpenAl Library: [https://www.openal.org/downloads/](https://www.openal.org/downloads/)

#### Build Instructions
1. Open CMD and set a path towards CMake if not yet done with (set PATH="C:\Program Files\CMake\bin";%PATH%)
2. Go to root/setup and run setup_x64.bat through the same console. This should open up a Visual Studio solution for you. If not, navigate to the newly created build folder and run the solution located in the `Voidfall` folder.

```
$ set PATH="PATH_TO_CMAKE_BIN";%PATH%
$ cd setup
$ setup_x64.bat

```
3. Build the Visual Studio solution in both Release and Debug. **NOTE:** Remember to build it for x64 if you use 64-bit system.
4. Exit the Visual Studio solution.
5. You should now be able to develop and build the project using the original VS solution located at `Voidfall/Voidfall.sln`

## External Links
This project makes liberal use of the following projects:
* bgfx - rendering engine: [https://github.com/bkaradzic/bgfx](https://github.com/bkaradzic/bgfx)
* GLFW - OpenGL library: [https://www.glfw.org/](https://www.glfw.org/)
* OpenAL - sound library: [https://www.openal.org/](https://www.openal.org/)
	- The implementation used: [https://openal-soft.org](https://openal-soft.org)
