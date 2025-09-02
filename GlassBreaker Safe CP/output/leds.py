"""
LED Output Management Module

This module handles all LED control functionality for the NATHAN Console.
It provides a clean interface for controlling individual LEDs and LED patterns.
"""

import board
import digitalio
import time


class LEDManager:
    """
    Manages all LED outputs for the NATHAN Console.
    
    This class provides methods to control individual LEDs and execute LED patterns
    for visual feedback during gameplay.
    """
    
    def __init__(self):
        """Initialize all LED outputs."""
        # Raspberry Pi onboard LED
        self.led_raspberry25 = digitalio.DigitalInOut(board.LED)
        self.led_raspberry25.direction = digitalio.Direction.OUTPUT
        
        # Game indicator LED
        self.lumiere_pin12 = digitalio.DigitalInOut(board.GP12)
        self.lumiere_pin12.direction = digitalio.Direction.OUTPUT
        
        # Game button LEDs
        self.lumiere_pin15 = digitalio.DigitalInOut(board.GP18)
        self.lumiere_pin15.direction = digitalio.Direction.OUTPUT
        
        self.lumiere_pin13 = digitalio.DigitalInOut(board.GP17)
        self.lumiere_pin13.direction = digitalio.Direction.OUTPUT
        
        self.lumiere_pin14 = digitalio.DigitalInOut(board.GP16)
        self.lumiere_pin14.direction = digitalio.Direction.OUTPUT
        
        self.lumiere_pin9 = digitalio.DigitalInOut(board.GP19)
        self.lumiere_pin9.direction = digitalio.Direction.OUTPUT
        
        self.lumiere_pin10 = digitalio.DigitalInOut(board.GP20)
        self.lumiere_pin10.direction = digitalio.Direction.OUTPUT
        
        # LED mapping for easier access
        self.led_pins = {
            "pin9": self.lumiere_pin9,
            "pin10": self.lumiere_pin10,
            "pin13": self.lumiere_pin13,
            "pin14": self.lumiere_pin14,
            "pin15": self.lumiere_pin15
        }
        
        # Initialize with startup pattern
        self._startup_pattern()
    
    def _startup_pattern(self):
        """Execute startup LED pattern to indicate system ready."""
        self.led_raspberry25.value = True
        time.sleep(1)
        self.led_raspberry25.value = False
    
    def set_led(self, pin_name, state):
        """
        Set the state of a specific LED.
        
        Args:
            pin_name (str): The pin name (e.g., "pin9", "pin10", etc.)
            state (bool): True to turn on, False to turn off
        """
        if pin_name in self.led_pins:
            self.led_pins[pin_name].value = state
    
    def turn_on_led(self, pin_name):
        """
        Turn on a specific LED.
        
        Args:
            pin_name (str): The pin name to turn on
        """
        self.set_led(pin_name, True)
    
    def turn_off_led(self, pin_name):
        """
        Turn off a specific LED.
        
        Args:
            pin_name (str): The pin name to turn off
        """
        self.set_led(pin_name, False)
    
    def turn_off_all_game_leds(self):
        """Turn off all game button LEDs."""
        for pin_name in self.led_pins:
            self.set_led(pin_name, False)
    
    def turn_on_all_game_leds(self):
        """Turn on all game button LEDs."""
        for pin_name in self.led_pins:
            self.set_led(pin_name, True)
    
    def set_game_indicator(self, state):
        """
        Set the game indicator LED state.
        
        Args:
            state (bool): True to indicate game in progress, False otherwise
        """
        self.lumiere_pin12.value = state
    
    def execute_sequence_pattern(self):
        """
        Execute a sequential LED pattern for visual feedback.
        This pattern lights up LEDs in sequence from left to right.
        """
        sequence = ["pin13", "pin14", "pin15", "pin10", "pin9"]
        for pin_name in sequence:
            self.turn_on_led(pin_name)
            time.sleep(0.2)
            self.turn_off_led(pin_name)
    
    def flash_led(self, pin_name, duration=0.25):
        """
        Flash a specific LED for a short duration.
        
        Args:
            pin_name (str): The pin name to flash
            duration (float): Flash duration in seconds
        """
        self.turn_on_led(pin_name)
        time.sleep(duration)
        self.turn_off_led(pin_name)