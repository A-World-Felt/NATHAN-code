"""
Button Input Management Module

This module handles all button input functionality for the NATHAN Console.
It provides a clean interface for reading button states and managing input events.
"""

import board
import digitalio


class ButtonManager:
    """
    Manages all button inputs for the NATHAN Console.
    
    This class initializes and provides access to all hardware buttons,
    making it easy to check button states throughout the application.
    """
    
    def __init__(self):
        """Initialize all button inputs with proper pull-down configuration."""
        # Start game button (pin 11)
        self.debuter_partie11 = digitalio.DigitalInOut(board.GP11)
        self.debuter_partie11.direction = digitalio.Direction.INPUT
        self.debuter_partie11.pull = digitalio.Pull.DOWN
        
        # Game buttons (pins 9, 10, 13, 14, 15)
        self.bouton_pin15 = digitalio.DigitalInOut(board.GP15)
        self.bouton_pin15.direction = digitalio.Direction.INPUT
        self.bouton_pin15.pull = digitalio.Pull.DOWN
        
        self.bouton_pin14 = digitalio.DigitalInOut(board.GP14)
        self.bouton_pin14.direction = digitalio.Direction.INPUT
        self.bouton_pin14.pull = digitalio.Pull.DOWN
        
        self.bouton_pin13 = digitalio.DigitalInOut(board.GP13)
        self.bouton_pin13.direction = digitalio.Direction.INPUT
        self.bouton_pin13.pull = digitalio.Pull.DOWN
        
        self.bouton_pin10 = digitalio.DigitalInOut(board.GP10)
        self.bouton_pin10.direction = digitalio.Direction.INPUT
        self.bouton_pin10.pull = digitalio.Pull.DOWN
        
        self.bouton_pin9 = digitalio.DigitalInOut(board.GP9)
        self.bouton_pin9.direction = digitalio.Direction.INPUT
        self.bouton_pin9.pull = digitalio.Pull.DOWN
        
        # Button pin mapping for easier access
        self.button_pins = {
            "pin9": self.bouton_pin9,
            "pin10": self.bouton_pin10,
            "pin13": self.bouton_pin13,
            "pin14": self.bouton_pin14,
            "pin15": self.bouton_pin15
        }
    
    def get_button_state(self, pin_name):
        """
        Get the current state of a specific button.
        
        Args:
            pin_name (str): The pin name (e.g., "pin9", "pin10", etc.)
            
        Returns:
            bool: True if button is pressed, False otherwise
        """
        if pin_name in self.button_pins:
            return self.button_pins[pin_name].value
        return False
    
    def any_game_button_pressed(self):
        """
        Check if any of the main game buttons are currently pressed.
        
        Returns:
            bool: True if any game button is pressed, False otherwise
        """
        return (self.bouton_pin15.value or self.bouton_pin14.value or 
                self.bouton_pin13.value or self.bouton_pin10.value or 
                self.bouton_pin9.value)
    
    def get_pressed_button(self):
        """
        Get the name of the currently pressed button.
        
        Returns:
            str or None: The pin name of the pressed button, or None if no button is pressed
        """
        for pin_name, button in self.button_pins.items():
            if button.value:
                return pin_name
        return None
    
    def wait_for_button_release(self, pin_name):
        """
        Wait for a specific button to be released.
        
        Args:
            pin_name (str): The pin name to wait for release
        """
        if pin_name in self.button_pins:
            while self.button_pins[pin_name].value:
                pass