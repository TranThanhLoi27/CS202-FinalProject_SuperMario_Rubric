# 2D Co-op Combat Platformer

A C++17/SFML 3 platformer for one or two players on the same computer. Choose a character profile, fight through four maps, manage health and hunger, collect items, place blocks, activate checkpoints, and reach the goal together.

This project was developed for the CS202 Object-Oriented Programming final project.

## Features

- One-player and two-player local co-op.
- Four maps:
  - Forest Trail
  - Stone Bridge
  - Spike Valley
  - Boss Lair
- Three difficulty settings that increase enemy health.
- Six character profiles with different traits:
  - Ori — higher jump
  - Rush — faster movement
  - Guard — blocks the first hit
  - Titan — additional health
  - Feather — slower falling
  - Legend — combines all traits and is unlocked through the shop
- Four enemy archetypes:
  - Patrolling slime
  - Stationary mushroom shooter
  - Flying bat
  - Two-phase golem boss
- Melee combat, dodging, enemy projectiles, knockback, hurt states, and death animations.
- Six-slot inventory containing food, coins, and placeable blocks.
- Hunger, starvation damage, food consumption, checkpoints, respawning, and tombstones.
- Player-placed blocks can be reclaimed without destroying natural map geometry.
- Mouse-driven menus with hover/click interaction.
- Runtime key rebinding for both players.
- Shop, wallet, character upgrades, unlockable content, and achievements.
- Sound effects and looping background music.
- Automatic save/load through `data.txt`.

## Technology

- C++17
- SFML 3
  - Graphics
  - Window
  - System
  - Audio
- CMake 3.28 or newer

## Repository layout

```text
.
|-- assets/
|   |-- audio/       Sound effects and background music
|   |-- fonts/       UI fonts
|   |-- levels/      Text-based level definitions
|   `-- textures/    Character, enemy, world, item, and UI textures
|-- include/
|   |-- Combat/
|   |-- Core/
|   |-- Entities/
|   |-- Graphics/
|   |-- UI/
|   |-- Utils/
|   `-- World/
|-- src/             C++ implementation files
|-- CMakelists.txt   CMake project definition
|-- project.md       Original project plan
|-- Report.md        Implementation report
|-- DESIGN_PATTERNS.md Pattern evidence and code references
`-- data.txt         Local progression save
```

## Prerequisites

Install the following before configuring the project:

1. A C++17-compatible compiler.
2. CMake 3.28 or newer.
3. SFML 3 built for the same compiler and architecture as your application.
4. A build tool such as Ninja, MinGW Makefiles, or Visual Studio.

The repository has been tested on Windows with:

- MinGW-w64
- Ninja
- SFML 3.0.2

Do not mix an SFML build compiled for MSVC with MinGW, or the reverse. The compiler, architecture, and SFML binaries must match.

## Build and run

### Windows — MinGW and Ninja

From PowerShell in the repository root:

```powershell
cmake -S . -B build -G Ninja `
  -DCMAKE_CXX_COMPILER=C:/path/to/mingw64/bin/g++.exe `
  -DSFML_DIR=C:/path/to/SFML/lib/cmake/SFML

cmake --build build
./build/sfml_CS202.exe
```

If MinGW and SFML are already available through your environment, the shorter configuration may work:

```powershell
cmake -S . -B build -G Ninja
cmake --build build
./build/sfml_CS202.exe
```

### Windows — Visual Studio

Use an SFML package built for the matching Visual Studio toolchain:

```powershell
cmake -S . -B build-vs -G "Visual Studio 17 2022" -A x64 `
  -DSFML_DIR=C:/path/to/SFML/lib/cmake/SFML

cmake --build build-vs --config Debug
./build-vs/Debug/sfml_CS202.exe
```

For a release build, replace `Debug` with `Release`.

### Linux or macOS

After installing an SFML 3 development package discoverable by CMake:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/sfml_CS202
```

If CMake cannot locate SFML, pass its CMake package directory explicitly:

```bash
cmake -S . -B build -DSFML_DIR=/path/to/SFML/lib/cmake/SFML
```

### Asset location

The CMake post-build step copies `assets/` beside the executable. You can therefore run from:

- The repository root, where the original `assets/` directory exists; or
- The executable output directory, where CMake copied the assets.

The save file is read and written relative to the program's current working directory. Avoid launching the executable from an unrelated directory if you want predictable save-file placement.

## How to start a game

The menus are mouse-driven:

1. Move the mouse over a button to highlight it.
2. Use the mouse wheel when a menu contains more entries than can be shown.
3. Left-click **1 Player** or **2 Players**.
4. Select an unlocked map.
5. Select a character for Player 1.
6. In two-player mode, select a character for Player 2.
7. The level starts after the final character selection.

Difficulty can be changed from **Choose Difficulty** on the main menu. Coins earned during play can be spent in the **Shop** on profile upgrades and the Legend character.

Selecting **Reset Game** clears saved progression, upgrades, unlocks, wallet coins, and achievements.

## Default controls

### Gameplay

| Action | Player 1 | Player 2 |
| --- | --- | --- |
| Move left | `A` | `Left Arrow` |
| Move right | `D` | `Right Arrow` |
| Jump | `W` | `Up Arrow` |
| Attack | `F` | `J` |
| Dodge | `S` | `Down Arrow` |
| Use selected item | `H` | `L` |
| Previous inventory slot | `Q` | `U` |
| Next inventory slot | `E` | `I` |
| Pause | `P` or `Escape` | `K` |

Player 1 can also press `1` through `6` to select an inventory slot directly. Player 2 changes slots with `U` and `I`.

### Inventory actions

| Slot | Content | Use-item behavior |
| ---: | --- | --- |
| 1–4 | Food variants | Consume one item, restore hunger, and heal one health point |
| 5 | Coins | Throw one coin in the facing direction |
| 6 | Blocks | Place one solid block in a valid empty tile ahead |

Attacking a player-placed block directly in front of the character reclaims it and returns one block to that player's inventory. Natural terrain cannot be reclaimed.

## Changing key bindings

Key bindings can be changed while a level is running:

1. Pause the game with `Escape`, `P`, or Player 2's `K`.
2. Left-click **Change Controls**.
3. Click the **Player 1 Controls** or **Player 2 Controls** selector.
4. Scroll to the required action.
5. Left-click the action row.
6. Press the new key.

Press `Escape` while waiting for a key to cancel the operation. A key already assigned to another action is rejected.

Key changes currently apply to the running session and are not stored in `data.txt`.

## How to play

### Objective

Travel through the level and reach the goal gate. In two-player mode, both active players normally need to enter the goal area.

Along the way:

- Defeat enemies using melee attacks.
- Dodge enemy contact and projectiles.
- Collect dropped food, coins, and blocks.
- Keep hunger above zero by consuming food.
- Use blocks to create temporary paths or protection.
- Activate checkpoints to update the respawn position.
- Coordinate movement so both players remain within the shared-camera area.

The Boss Lair ends with a golem that changes behavior at half health. Its enraged phase moves faster, attacks more frequently, and creates ground shockwaves.

### Health, death, and recovery

- Enemy attacks, projectiles, spikes, and starvation can reduce health.
- Hunger drains continuously; reaching zero causes starvation damage.
- A dead player leaves their inventory in a tombstone.
- The player respawns after a short delay at the latest checkpoint or safe spawn.
- Touch the tombstone to recover its stored inventory.

### Progression

- Completing levels unlocks progression and awards coins.
- Wallet coins persist between runs.
- The shop can upgrade character health and unlock Legend.
- Achievements are tracked and saved automatically.
- Progress is loaded when the game starts and saved when the application closes.

## Level format

Levels are stored as text files under `assets/levels/`. The loader converts map symbols into terrain and gameplay objects such as:

| Symbol | Meaning |
| --- | --- |
| `#` | Solid terrain |
| `1` | Player 1 spawn |
| `2` | Player 2 spawn |
| `P` | Patrol enemy |
| `R` | Ranged mushroom enemy |
| `F` | Flying enemy |
| `B` | Boss enemy |
| `C` | Checkpoint |
| `^` | Spike |
| `G` | Goal gate |

See the existing level files and `LevelLoader` implementation before editing a map.

## Troubleshooting

### CMake cannot find SFML

Pass `SFML_DIR` during configuration:

```powershell
cmake -S . -B build -DSFML_DIR=C:/path/to/SFML/lib/cmake/SFML
```

The target directory should contain SFML's CMake configuration files.

### Missing DLL error on Windows

Ensure the SFML and OpenAL runtime DLLs for your compiler configuration are:

- Beside `sfml_CS202.exe`; or
- Available through the system `PATH`.

Debug and release builds may require different SFML runtime binaries.

### “Could not load required textures”

Run the game from the repository root or from the CMake output directory containing the copied `assets/` folder. Verify that Git downloaded the binary texture, audio, and font files correctly.

### The executable uses an unexpected save file

`data.txt` is relative to the current working directory. Launch the game from a consistent directory, or configure your IDE's working directory to the repository root/output directory.

### Reconfigure after changing compiler or SFML installation

Use a new build directory instead of reusing a cache created for another toolchain:

```powershell
cmake -S . -B build-new -G Ninja -DSFML_DIR=C:/path/to/SFML/lib/cmake/SFML
cmake --build build-new
```

## Documentation

- [Project plan](project.md)
- [Implementation report](Report.md)
- [Design pattern evidence](DESIGN_PATTERNS.md)
- [AI usage declaration](25125042_AI%20Usage%20Declaration.md)

## Academic use

This repository is an academic project. No standalone license file is currently included. Contact the project authors before reusing code or assets outside the course context.
