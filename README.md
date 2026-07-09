# FRIK'n Smooth Movement Only

Smooth-movement-only Fallout 4 VR F4SE plugin, rebuilt from FRIK's integrated SmoothMovementVR code.

This project intentionally excludes FRIK's body, Pip-Boy, holotape/Papyrus API, weapon positioning, flashlight, mesh, script, ESP, and UI/resource systems. The plugin installs one hook: the Fallout 4 VR smooth movement hook.

## Config

The mod ships its config as `F4SE/Plugins/FRIK.ini`, next to `FRIK.dll`. At runtime that resolves to `Data/F4SE/Plugins/FRIK.ini`.

If `Data/F4SE/Plugins/FRIK.ini` is missing, the plugin creates a default one automatically.

## Logging

The plugin writes a single truncating `FRIK.log` in the normal Fallout 4 VR F4SE log folder.

## Build

```bat
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
set "VCPKG_ROOT=C:\Fallout 4 VR\build\vcpkg"
"C:\Program Files\CMake\bin\cmake.exe" -S . -B build-smooth -G Ninja -DCMAKE_BUILD_TYPE=Release
"C:\Program Files\CMake\bin\cmake.exe" --build build-smooth --config Release --parallel 2
```

The compiled plugin is `build-smooth/FRIK.dll`. The ready-to-use MO2 layout is in `dist/FRIK'n Smooth Movement Only`.
