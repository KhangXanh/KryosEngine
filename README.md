# Kryos Engine

<p align="center">
  <img src="Source/Logo.png" width="500">
</p>

<p align="center">
  <a href="https://en.cppreference.com/w/cpp/23"><img src="https://img.shields.io/badge/C%2B%2B-23-blue.svg" alt="C++23"></a>
  <a href="LICENSE"><img src="https://img.shields.io/badge/License-Apache_2.0-green.svg" alt="License"></a>
  <img src="https://img.shields.io/badge/Platform-Windows-lightgrey.svg" alt="Platform">
</p>

A game engine written in C++23. Still early in development.

## What's in so far

- Multi-window support
- Layer stack system
- Event bus system
- Input system

## Getting Started

Requires:
- C++23 Compatible Compiler (MSVC/GCC)
- VS Code (Recommended)
- CMake (3.20 or higher)
- Git

Start by cloning the repository with:
```bash
git clone https://github.com/KhangXanh/KryosEngine/
cd KryosEngine
```

Then run `Setup.bat` to configure, and `Build.bat` in the `Scripts` folder to compile:
```bash
.\Scripts\Setup.bat
.\Scripts\Build.bat
```

## Roadmap

- [x] Multi-window system
- [x] Application inheritance
- [x] Event system
- [x] Input system
- [ ] Task manager (threading)
- [X] Entity Component System
- [ ] Scene manager
- [ ] 2D Renderer
- [ ] UIce framework
- [ ] 3D Renderer
- [ ] Ray tracing
- [ ] Lua scripting
- [ ] Build pipeline

## Credits

Special thanks to [TheCherno's Hazel Engine](https://github.com/TheCherno/Hazel) for inspiring me to create this game engine.

## License

Kryos Engine is licensed under the Apache License, Version 2.0.

See the [LICENSE](LICENSE) file for details.