# Implementation Plan - ESC Pause Menu, Key Rebinding & Fairy Companion System

Implement a Pause Menu accessible via ESC during gameplay with Key Rebinding support using custom selector frames, alongside a Fairy Companion system that flies around characters with dynamic fast-movement animations.

## User Review Required

> [!IMPORTANT]
> - **Key Rebinding**: Clicking/selecting a control action will prompt the player to press any **unassigned key**. If the key is already used by another action, a warning will be displayed and only unassigned keys will be accepted.
> - **UI Framing**: Unselected menu frames will use [`selector1.png`](file:///d:/CS202/assets/textures/selector1.png), while currently selected/active frames will use [`selector2.png`](file:///d:/CS202/assets/textures/selector2.png).
> - **Fairy Companions**: Each active character will have 2-3 glowing fairies floating around them in an orbit during normal movement, and trailing dynamically with fast-fluttering motion trails during fast movement.

## Proposed Changes

### Core & Input System

#### [MODIFY] [InputManager.h](file:///d:/CS202/include/Core/InputManager.h)
#### [MODIFY] [InputManager.cpp](file:///d:/CS202/src/Core/InputManager.cpp)
- Replace static key polling with dynamic, rebindable key structures for Player 1 and Player 2.
- Add helper methods:
  - `rebindKey(playerIndex, actionIndex, newKey)`
  - `isKeyAssigned(sf::Keyboard::Key key) const`
  - `getKeyName(sf::Keyboard::Key key)` for user-friendly UI display.

#### [MODIFY] [Game.h](file:///d:/CS202/include/Core/Game.h)
#### [MODIFY] [Game.cpp](file:///d:/CS202/src/Core/Game.cpp)
- Load `fairy.png`, `selector1.png`, and `selector2.png` textures into `AssetManager`.
- Update state handling in `Game::update` so pressing `ESC` during `GameState::Playing` toggles `GameState::Paused`.
- Add Pause Submenus (Pause Main Menu, Key Rebind Menu) and handle navigation / rebinding key capture.

---

### UI System

#### [MODIFY] [MenuScreen.h](file:///d:/CS202/include/UI/MenuScreen.h)
#### [MODIFY] [MenuScreen.cpp](file:///d:/CS202/src/UI/MenuScreen.cpp)
- Update `setSelectorTextures(selector1, selector2)` to store both frame textures.
- Update `MenuScreen::row(...)` so:
  - Unselected rows render with [`selector1.png`](file:///d:/CS202/assets/textures/selector1.png).
  - Currently selected / active row renders with [`selector2.png`](file:///d:/CS202/assets/textures/selector2.png).
- Add UI rendering for the **Pause Menu** and **Key Rebinding Menu**.

---

### Fairy Companion System

#### [NEW] [FairyCompanion.h](file:///d:/CS202/include/Entities/FairyCompanion.h)
#### [NEW] [FairyCompanion.cpp](file:///d:/CS202/src/Entities/FairyCompanion.cpp)
- Create `FairyCompanionManager` / `FairyCompanion` class:
  - Spawns 3 fairies per player using [`fairy.png`](file:///d:/CS202/assets/textures/fairy.png).
  - **Normal / Idle State**: Smooth orbital floating & bobbing around player center.
  - **Fast Movement State**: When character speed exceeds threshold (e.g. running, dashing, fast falling), fairies stream behind the player's movement direction, flutter rapidly, and emit fading sparkle trail particles.

#### [MODIFY] [Player.h](file:///d:/CS202/include/Entities/Player.h)
#### [MODIFY] [Player.cpp](file:///d:/CS202/src/Entities/Player.cpp)
#### [MODIFY] [Level.h](file:///d:/CS202/include/World/Level.h)
#### [MODIFY] [Level.cpp](file:///d:/CS202/src/World/Level.cpp)
- Attach fairy companions to players and handle update/render calls during gameplay.

## Verification Plan

### Automated Build Verification
- Compile the codebase using CMake / MSVC compiler:
  ```powershell
  cmake --build d:/CS202/build --config Release
  ```

### Manual Verification
- Test pressing `ESC` during gameplay to verify the Pause Menu appears.
- Test selecting "Change Controls" and rebind a key:
  - Verify pressing an unassigned key successfully updates the key binding.
  - Verify pressing an already assigned key displays a warning.
- Verify UI frames: check that unselected items display `selector1.png` and selected items display `selector2.png`.
- Test fairy companions: observe 3 fairies orbiting characters during slow movement, and streaming trailing particle animations when running/dashing fast.
