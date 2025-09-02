---
description: "project guidelines"
applyTo: "**/*.py"
---

# Project Refactoring Guide for NATHAN Console

## Objective

Refactor the NATHAN Console project to separate functionalities into dedicated files, following a clean architecture. The goal is to create a library that makes it easy to program accessible games for blind or visually impaired users.

---

## Steps for the GitHub Agent

### 1. Analyze Existing Code
- Identify different functionalities (button management, LEDs, audio, game logic, etc.) in the main files (`code.py`, `Memoire.py`, etc.).
- Locate dependencies and used modules.

### 2. Define Clean Architecture
Organize code into modules/files by responsibility:
- `input/` : user input and button management
- `output/` : LED and audio management
- `game/` : game logic, states, scenarios
- `utils/` : utility functions and helpers
- `main.py` : minimal entry point that imports and uses the modules

### 3. Refactoring
- Create folders and files according to the defined architecture.
- Move each functionality into its dedicated module.
- Ensure each module exposes a simple, clear API (e.g., `ButtonManager`, `AudioManager`, `LEDManager`, etc.).
- Adapt the code so that the entry point (`main.py`) only uses the modules, with no direct business logic.

### 4. Documentation
- Add docstrings and comments for each module.
- Write a README or usage guide explaining how to program a game using the library.

### 5. Testing
- (Optional) Add examples or unit tests to validate module functionality.

---

## Target Directory Structure Example

```
lib/
    input/
        buttons.py
    output/
        leds.py
        audio.py
    game/
        engine.py
        scenario.py
    utils/
        helpers.py
main.py
README.md
```

---

## Best Practices
- Respect separation of concerns.
- Use explicit names for modules and functions.
- Make it easy to extend for new games or features.
- Keep code accessible and well-commented.

---

## Agent Mission

> Refactor the project according to the architecture above, separating each functionality into a dedicated module, and document the API so the community can easily create accessible games.

---

## Menu System Instructions

- The project is designed as a game console, so users should be able to select games from a menu.
- For now, since menu sounds are not yet created, only the existing game should be available and selectable.
- Implement a basic menu structure that defaults to launching the current game.
- Prepare the code so that adding new games and menu sounds in the future will be straightforward.
- Document the menu logic and how to extend it for future games and audio assets.

---

Ready to start refactoring!
