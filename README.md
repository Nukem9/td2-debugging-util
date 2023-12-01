# The Division 2 Debugging Utility

A reverse engineering aid that disables EAC for the Uplay edition of The Division 2. This project was created for debugging purposes only and **will not** allow you to play online. No support is provided.

## Building

- CMake and vcpkg are expected to be set up beforehand. Visual Studio 2022 is recommended.
- Clone this repository and any submodules.
- Navigate to your game folder and rename `dbgcore.dll` to `dbgcore_old.dll`.
- Navigate to your game folder and create backup copies of `EasyAntiCheat_x86.dll` and `EasyAntiCheat_x64.dll`. These are **overwritten** at build time.
- Edit `CMakeUserEnvVars.json` and set `GAME_ROOT_DIRECTORY` to The Division 2's root directory. The build script automatically copies library files to the game folder.
- Open CMakeLists.txt in Visual Studio and build both the **x86 and amd64 architectures**.

## License

- [GPLv3](LICENSE.md)