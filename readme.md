# Penrose Engine #

Penrose Engine is my own game engine built from scratch and written in C++20.

## Build & Run ##

Penrose uses Meson as build system. To build Penrose you can execute the following command:

```shell
meson setup builddir && meson compile -C builddir
```

Penrose comes with some tests to check functionality of the engine.

You need to convert raw assets to format that Penrose accepts before executing tests. You can use Penrose assets packer
from [utilities repo](https://github.com/siberianbot/penrose-utils). Then you can run tests with the following command:

```shell
meson test -C builddir --verbose
```

## Limitations ##

* Development of engine is done on Linux-based machine and engine wasn't compiled and tested to run on anything
  different from Linux (but it's possible to build and run wherever GLFW and Vulkan available due to usage of
  cross-platform libraries)

## Dependencies ##

### Engine ###

* Vulkan

* [fmt](https://github.com/fmtlib/fmt)
* [GLFW](https://github.com/glfw/glfw)
* [Dear ImGui](https://github.com/ocornut/imgui)
* [OpenGL Mathematics](https://github.com/g-truc/glm)
* [Catch2](https://github.com/catchorg/Catch2)

### [Utilities](https://github.com/siberianbot/penrose-utils) ###

* [Silk.NET.Assimp](https://github.com/dotnet/Silk.NET)
* [StbImageSharp](https://github.com/StbSharp/StbImageSharp)
* [System.CommandLine](https://github.com/dotnet/command-line-api)
