# FRIK Smooth Movement

A standalone **Fallout 4 VR** F4SE plugin that provides only FRIK's *smooth movement* feature —
the position smoothing that masks the up/down and horizontal stutter of artificial (thumbstick)
locomotion — with none of FRIK's body, Pip-Boy, weapon, UI, Papyrus, or mesh systems.

It is built on the [F4VR Common Framework](https://github.com/ArthurHub/F4VR-CommonFramework) and
ships as a single `FRIK-SmoothMovement.dll`. It is fully independent of FRIK: different plugin name,
INI, and log, so the two can coexist (though there is little reason to run both — FRIK already
includes this smoothing).

The smoothing code is adapted from the original FRIK implementation, itself based on code by Shizof
(used with permission).

## How it works

The plugin hooks the game's dedicated smooth-movement call site and, each movement tick, nudges the
player world node toward a smoothed position before the game applies its own movement. That is the
same injection point FRIK uses, which is why the result matches FRIK's smoothing exactly.

## Config

Settings live in a hot-reloadable INI, created automatically on first run from a copy embedded in
the DLL:

```
Documents\My Games\Fallout4VR\Mods_Config\FRIK-SmoothMovement\FRIK-SmoothMovement.ini
```

Edit it while the game is running and the changes apply immediately — no restart. Keys (all under
the `[FRIK-SmoothMovement]` section):

| Key                             | Default | Meaning                                            |
| ------------------------------- | ------- | -------------------------------------------------- |
| `fSmoothingAmount`              | `5.0`   | Vertical (up/down) smoothing; higher = smoother    |
| `fDamping`                      | `1.0`   | Vertical damping                                   |
| `fStoppingMultiplier`           | `0.5`   | Vertical smoothing while standing still            |
| `fSmoothingAmountHorizontal`    | `5.0`   | Horizontal smoothing; higher = smoother            |
| `fDampingHorizontal`            | `1.0`   | Horizontal damping                                 |
| `fStoppingMultiplierHorizontal` | `0.5`   | Horizontal smoothing while standing still          |

The `[Debug]` section controls the log level/pattern (see the framework's
[debug-config guide](external/F4VR-CommonFramework/docs/debug-config.md)).

## Logging

The plugin writes a rotating `FRIK-SmoothMovement.log` in the standard Fallout 4 VR F4SE log folder:

```
Documents\My Games\Fallout4VR\F4SE\FRIK-SmoothMovement.log
```

## Build

Prerequisites (same as the framework — `VCPKG_ROOT`, VS 2022/2026 x64, CMake 4.2+):

```sh
git submodule update --init --recursive   # pulls the framework and its submodules

cp CMakeUserPresets.json.template CMakeUserPresets.json   # then edit COPY_PLUGIN_BASE_PATH
cmake --preset custom                                     # or: cmake --preset default
```

Open `build/FRIK-SmoothMovement.sln`, pick **Debug** or **Release**, and build. A Release build
also stages `data/mod/` + the DLL and produces a versioned `.7z` under `build/package/` ready for a
mod manager. If you set `COPY_PLUGIN_BASE_PATH`, the DLL/PDB are copied into your game/MO2 folder
after every build.

## Install

Drop `FRIK-SmoothMovement.dll` into `Fallout4VR\Data\F4SE\Plugins\` (or install the packaged `.7z`
with a mod manager) and launch through `f4sevr_loader.exe`.
