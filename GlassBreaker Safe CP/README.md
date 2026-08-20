# NATHAN Console - Documentation

## Overview

The NATHAN Console is a button-based reaction game system built for CircuitPython microcontrollers. This document describes the refactored modular architecture and how to extend the system for new games and features.

## Architecture

The system has been refactored into a clean, modular architecture with the following structure:

```
GlassBreaker Safe CP/
├── main.py                 # Main entry point
├── input/                  # Input handling modules
│   ├── __init__.py
│   └── buttons.py         # Button input management
├── output/                 # Output handling modules  
│   ├── __init__.py
│   ├── leds.py           # LED control
│   ├── audio.py          # Audio playback
│   └── motors.py         # Motor/vibration control
├── game/                   # Game logic modules
│   ├── __init__.py
│   ├── logic.py          # Core game logic
│   └── menu.py           # Menu system
├── utils/                  # Utility modules
│   ├── __init__.py
│   └── memory.py         # Memory/storage management
├── lib/                    # CircuitPython libraries
├── sounds/                 # Audio files
└── [legacy files]          # Original files (for reference)
```

## Modules

### Input Module (`input/`)

**`buttons.py`** - Button Input Management
- `ButtonManager` class handles all button inputs
- Provides clean interface for checking button states
- Manages button debouncing and state tracking

Key Methods:
- `get_button_state(pin_name)` - Check if a button is pressed
- `any_game_button_pressed()` - Check if any game button is active
- `get_pressed_button()` - Get the currently pressed button name

### Output Module (`output/`)

**`leds.py`** - LED Control
- `LEDManager` class controls all LED outputs
- Manages individual LEDs and LED patterns
- Provides visual feedback for gameplay

Key Methods:
- `set_led(pin_name, state)` - Control individual LEDs
- `execute_sequence_pattern()` - Run LED sequences
- `turn_off_all_game_leds()` - Clear all game LEDs

**`audio.py`** - Audio Management
- `AudioManager` class handles all audio playback
- Manages sound effects, music, and audio feedback
- Supports interruible and non-interruible sounds

Key Methods:
- `play_sound(filename)` - Play a sound file
- `play_hit_sound(pin_name)` - Play button hit sounds
- `play_mode_sound(mode, pin_name)` - Play mode-specific sounds
- `play_menu_music()` - Handle menu navigation with music

**`motors.py`** - Motor/Vibration Control
- `MotorManager` class controls vibration motors
- Provides haptic feedback for game events
- Supports individual and combined motor control

Key Methods:
- `vibrate_for_button(pin_name, is_correct)` - Context-aware vibration
- `vibrate_left()`, `vibrate_right()`, `vibrate_both()` - Direct control

### Game Module (`game/`)

**`logic.py`** - Core Game Logic
- `GameState` class tracks current game status
- `GameLogic` class orchestrates gameplay flow
- Manages scoring, timing, and game modes

Key Methods:
- `start_game()` - Begin a new game session
- `set_game_mode(mode)` - Change the active game mode
- `get_current_score()` - Get current score

**`menu.py`** - Menu System
- `MenuSystem` class provides main menu interface
- Handles game mode selection and navigation
- `MenuExtension` class for future menu expansion

Key Methods:
- `show_main_menu()` - Display main menu and handle input
- `set_default_mode(mode)` - Set default game mode

### Utils Module (`utils/`)

**`memory.py`** - Memory Management
- `MemoryManager` class handles persistent storage
- Manages SD card operations and high scores
- Provides data persistence across power cycles

Key Methods:
- `read_high_score(mode)` - Get high score for a mode
- `write_high_score(mode, score)` - Update high score
- `get_all_high_scores()` - Get all high scores

## Menu System

The menu system provides a clean interface for:

1. **Main Menu**: Play menu music and wait for user input
   - Button 14: Start game with current mode
   - Buttons 9/10: Enter mode selection

2. **Mode Selection**: Cycle through available game modes
   - Button 9: Next mode
   - Button 10: Previous mode  
   - Button 15: Select current mode

### Available Game Modes

1. **Arcade** - Classic mode with tones for each button
2. **Voice** - Voice prompts for buttons
3. **Reverse** - Reverse audio cues
4. **Light** - Visual-only mode (no audio cues)
5. **Megamix** - Random mix of other modes

## Extending the System

### Adding New Games

1. **Add Game Mode Name**:
   ```python
   # In game/menu.py
   self.game_modes.append("NewGame")
   ```

2. **Create Audio Files**:
   - `ModeNewGame.wav` - Mode announcement
   - `1CNewGame.wav` through `5CNewGame.wav` - Button cues

3. **Implement Mode Logic** (if needed):
   ```python
   # In game/logic.py, modify _present_challenge() method
   elif self.game_state.game_mode == "NewGame":
       # Custom logic for new game mode
   ```

### Adding Menu Features

Use the `MenuExtension` class:

```python
from game.menu import MenuExtension

# Create extension
menu_ext = MenuExtension()

# Add custom menu item
def handle_settings():
    print("Settings menu")
    # Settings logic here

menu_ext.add_menu_item("Settings", handle_settings)
```

### Adding New Hardware

1. **For New Buttons**:
   - Add pins to `ButtonManager.__init__()`
   - Update button mapping dictionaries

2. **For New LEDs**:
   - Add pins to `LEDManager.__init__()`
   - Update LED mapping dictionaries

3. **For New Audio Output**:
   - Modify `AudioManager.__init__()`
   - Add new audio methods as needed

## Audio File Requirements

All audio files should be:
- Format: WAV (16-bit, mono, 22050 Hz recommended)
- Location: `/sd/sounds/` directory on SD card
- Naming convention:
  - Mode sounds: `Mode{ModeName}.wav`
  - Button sounds: `{1-5}C{ModeName}.wav` for commands
  - Hit sounds: `{1-5}H.wav` for successful hits
  - System sounds: `Title.wav`, `Score.wav`, etc.

## Testing

The system includes a test mode for hardware verification:

1. Set `TEST_MODE = True` in `main.py`
2. Run the system to test all components
3. Follow on-screen prompts to verify functionality

## Hardware Configuration

The system uses the following pin assignments:

### Buttons (Input)
- GP9, GP10, GP11, GP13, GP14, GP15

### LEDs (Output)  
- GP12 (game indicator)
- GP16, GP17, GP18, GP19, GP20 (game buttons)
- Built-in LED (status)

### Audio (Output)
- GP21 (PWM audio output)

### Motors (Output)
- GP26 (left motor)
- GP27 (right motor)

### SD Card (Storage)
- GP0 (MISO), GP1 (CS), GP2 (CLK), GP3 (MOSI)

## Migration from Legacy Code

The original `code.py` has been preserved for reference. The new modular system maintains all existing functionality while providing:

- Better code organization
- Easier maintenance and debugging
- Simplified extension for new features
- Improved error handling
- Better documentation

## Troubleshooting

### Common Issues

1. **SD Card Not Mounting**:
   - Check connections
   - Verify SD card format (FAT32)
   - Check for corrupt files

2. **Audio Not Playing**:
   - Verify audio files exist in `/sd/sounds/`
   - Check file format and naming
   - Ensure SD card is mounted

3. **Buttons Not Responding**:
   - Check wiring and pull-down resistors
   - Verify pin assignments
   - Test with hardware test mode

4. **LEDs Not Working**:
   - Check power supply
   - Verify pin assignments
   - Test individual LEDs

## Development Notes

- The system is designed for CircuitPython 8.x
- Memory usage is optimized for microcontroller constraints
- All modules use defensive programming practices
- Error handling preserves system stability
- The architecture supports hot-swapping of components for development

## Future Enhancements

The modular architecture makes these enhancements straightforward:

1. **Network Connectivity**: Add wifi module for online features
2. **Display Support**: Add screen output module
3. **More Game Modes**: Easy to add via menu extension
4. **Configuration System**: Settings persistence and customization
5. **Multiplayer Support**: Player management and networking
6. **Advanced Audio**: Music tracks, sound mixing
7. **Data Analytics**: Game statistics and analysis

## Support

For questions or issues with the NATHAN Console system, please refer to:
1. This documentation
2. Code comments in individual modules
3. Hardware test mode for diagnostics
4. Legacy code for reference implementation