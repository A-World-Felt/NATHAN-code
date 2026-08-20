"""
Menu System Module

This module provides the main menu interface for the NATHAN Console.
It handles game mode selection and navigation between different options.
"""

import time


class MenuSystem:
    """
    Main menu system for the NATHAN Console.
    
    This class provides a menu interface for selecting game modes
    and starting games. It's designed to be easily extensible for
    future menu options and features.
    """
    
    def __init__(self, button_manager, audio_manager, game_logic):
        """
        Initialize the menu system.
        
        Args:
            button_manager: Button input manager
            audio_manager: Audio output manager
            game_logic: Game logic controller
        """
        self.button_manager = button_manager
        self.audio_manager = audio_manager
        self.game_logic = game_logic
        
        # Available game modes
        self.game_modes = ["Arcade", "Voice", "Reverse", "Light", "Megamix"]
        self.current_mode_index = 0  # Start with Arcade mode
    
    def show_main_menu(self):
        """
        Display and handle the main menu.
        
        This method runs the main menu loop, handling user input
        for menu navigation and game starting.
        
        Returns:
            str: The action to take ("start_game" or "quit")
        """
        while True:
            # Play menu music and wait for input
            action = self.audio_manager.play_menu_music(self.button_manager)
            
            if action == "ModeDeJeu":
                # User wants to change game mode
                self._handle_mode_selection()
            elif action == "DebuterPartie":
                # User wants to start the game
                return "start_game"
            
            # Small delay before repeating menu
            time.sleep(0.1)
    
    def _handle_mode_selection(self):
        """
        Handle game mode selection interface.
        
        This method allows the user to cycle through available game modes
        and select one. The selection is confirmed with the select button.
        """
        print("Mode selection menu")
        
        # Play current mode announcement
        current_mode = self.game_modes[self.current_mode_index]
        self.audio_manager.play_mode_sound(current_mode, button_manager=self.button_manager)
        
        while True:
            # Check for mode navigation
            if self.button_manager.bouton_pin9.value:
                # Next mode
                self._next_mode()
                # Wait for button release to prevent rapid cycling
                self.button_manager.wait_for_button_release("pin9")
                time.sleep(0.2)  # Additional debounce
                
            elif self.button_manager.bouton_pin10.value:
                # Previous mode
                self._previous_mode()
                # Wait for button release to prevent rapid cycling
                self.button_manager.wait_for_button_release("pin10")
                time.sleep(0.2)  # Additional debounce
                
            elif self.button_manager.bouton_pin15.value:
                # Select current mode
                selected_mode = self.game_modes[self.current_mode_index]
                self.game_logic.set_game_mode(selected_mode)
                print(f"Selected mode: {selected_mode}")
                
                # Wait for button release
                self.button_manager.wait_for_button_release("pin15")
                return
            
            time.sleep(0.01)  # Small delay to prevent busy waiting
    
    def _next_mode(self):
        """Move to the next game mode in the list."""
        self.current_mode_index = (self.current_mode_index + 1) % len(self.game_modes)
        current_mode = self.game_modes[self.current_mode_index]
        print(f"Next mode: {current_mode}")
        self.audio_manager.play_mode_sound(current_mode, button_manager=self.button_manager)
    
    def _previous_mode(self):
        """Move to the previous game mode in the list."""
        self.current_mode_index = (self.current_mode_index - 1) % len(self.game_modes)
        current_mode = self.game_modes[self.current_mode_index]
        print(f"Previous mode: {current_mode}")
        self.audio_manager.play_mode_sound(current_mode, button_manager=self.button_manager)
    
    def get_current_mode(self):
        """
        Get the currently selected game mode.
        
        Returns:
            str: The currently selected game mode
        """
        return self.game_modes[self.current_mode_index]
    
    def set_default_mode(self, mode="Arcade"):
        """
        Set the default game mode.
        
        Args:
            mode (str): The mode to set as default
        """
        if mode in self.game_modes:
            self.current_mode_index = self.game_modes.index(mode)
            self.game_logic.set_game_mode(mode)
            print(f"Default mode set to: {mode}")


class MenuExtension:
    """
    Extension framework for adding new menu features.
    
    This class provides a framework for extending the menu system
    with new features, games, or options in the future.
    """
    
    def __init__(self):
        """Initialize menu extension framework."""
        self.custom_menu_items = []
        self.custom_handlers = {}
    
    def add_menu_item(self, name, handler):
        """
        Add a custom menu item.
        
        Args:
            name (str): Name of the menu item
            handler (callable): Function to handle the menu item selection
        """
        self.custom_menu_items.append(name)
        self.custom_handlers[name] = handler
    
    def handle_custom_item(self, item_name):
        """
        Handle selection of a custom menu item.
        
        Args:
            item_name (str): Name of the selected item
        """
        if item_name in self.custom_handlers:
            self.custom_handlers[item_name]()
    
    def get_custom_items(self):
        """
        Get list of custom menu items.
        
        Returns:
            list: List of custom menu item names
        """
        return self.custom_menu_items.copy()


# Future extension documentation:
"""
To add new games or menu features in the future:

1. Game Modes:
   - Add new mode name to MenuSystem.game_modes list
   - Ensure audio files exist for the mode (Mode{Name}.wav)
   - Implement mode-specific logic in GameLogic class

2. Menu Options:
   - Use MenuExtension class to add new menu items
   - Create handler functions for new options
   - Add corresponding audio files for new menu items

3. Audio Assets:
   - Menu sounds should be placed in /sd/sounds/ directory
   - Follow naming convention: Mode{GameName}.wav for game modes
   - Menu navigation sounds can be added to AudioManager

Example of adding a new game mode:
```python
# In main.py or initialization code:
menu_system.game_modes.append("NewGame")

# Ensure these files exist:
# /sd/sounds/ModeNewGame.wav
# /sd/sounds/1CNewGame.wav (for each button)
# etc.
```

Example of adding a new menu option:
```python
# Create handler function
def handle_settings():
    print("Settings menu opened")
    # Settings logic here

# Add to menu extension
menu_extension = MenuExtension()
menu_extension.add_menu_item("Settings", handle_settings)
```
"""