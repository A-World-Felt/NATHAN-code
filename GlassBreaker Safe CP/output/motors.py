"""
Motor/Vibration Output Management Module

This module handles motor and vibration feedback for the NATHAN Console.
It provides haptic feedback for different game events and user interactions.
"""

import board
import digitalio
import time


class MotorManager:
    """
    Manages motor/vibration outputs for the NATHAN Console.
    
    This class provides methods to control vibration motors for haptic feedback
    during gameplay.
    """
    
    def __init__(self):
        """Initialize motor outputs."""
        # Left motor
        self.motor_left = digitalio.DigitalInOut(board.GP26)
        self.motor_left.direction = digitalio.Direction.OUTPUT
        
        # Right motor
        self.motor_right = digitalio.DigitalInOut(board.GP27)
        self.motor_right.direction = digitalio.Direction.OUTPUT
    
    def vibrate_motor(self, motor, duration=0.25):
        """
        Vibrate a specific motor for a given duration.
        
        Args:
            motor: The motor object to vibrate
            duration (float): Vibration duration in seconds
        """
        motor.value = True
        time.sleep(duration)
        motor.value = False
    
    def vibrate_left(self, duration=0.25):
        """
        Vibrate the left motor.
        
        Args:
            duration (float): Vibration duration in seconds
        """
        self.vibrate_motor(self.motor_left, duration)
    
    def vibrate_right(self, duration=0.25):
        """
        Vibrate the right motor.
        
        Args:
            duration (float): Vibration duration in seconds
        """
        self.vibrate_motor(self.motor_right, duration)
    
    def vibrate_both(self, duration=0.25):
        """
        Vibrate both motors simultaneously.
        
        Args:
            duration (float): Vibration duration in seconds
        """
        self.motor_left.value = True
        self.motor_right.value = True
        time.sleep(duration)
        self.motor_left.value = False
        self.motor_right.value = False
    
    def vibrate_for_button(self, pin_name, is_correct=True):
        """
        Provide haptic feedback based on which button was pressed.
        
        Args:
            pin_name (str): The button that was pressed
            is_correct (bool): Whether the button press was correct
        """
        if not is_correct:
            # Wrong button - vibrate both motors for emphasis
            self.vibrate_both()
        elif pin_name in ["pin13", "pin10"]:
            # Left side buttons
            self.vibrate_left()
        elif pin_name == "pin14":
            # Center button - vibrate both
            self.vibrate_both()
        else:
            # Right side buttons (pin15, pin9)
            self.vibrate_right()
    
    def stop_all_motors(self):
        """Stop all motor vibrations immediately."""
        self.motor_left.value = False
        self.motor_right.value = False