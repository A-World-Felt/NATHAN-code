"""
Game Logic Module

This module contains the core game logic for the NATHAN Console.
It manages game states, scoring, timing, and game mode implementations.
"""

import time
import random


class GameState:
    """
    Represents the current state of a game session.
    
    This class tracks all the important game state information including
    score, game mode, current challenge, and game status.
    """
    
    def __init__(self, game_mode="Arcade"):
        """
        Initialize a new game state.
        
        Args:
            game_mode (str): The game mode to initialize
        """
        self.game_mode = game_mode
        self.score = 0
        self.current_target = None
        self.is_active = False
        self.is_game_over = False
        self.high_scores = {}
        
        # Available pin options for challenges
        self.available_pins = ["pin13", "pin14", "pin15", "pin10", "pin9"]
    
    def reset(self):
        """Reset the game state for a new game."""
        self.score = 0
        self.current_target = None
        self.is_active = False
        self.is_game_over = False
    
    def start_game(self):
        """Start a new game session."""
        self.reset()
        self.is_active = True
        self.is_game_over = False
    
    def end_game(self):
        """End the current game session."""
        self.is_active = False
        self.is_game_over = True
    
    def generate_challenge(self):
        """
        Generate a new challenge (target button).
        
        Returns:
            str: The pin name for the new challenge
        """
        self.current_target = random.choice(self.available_pins)
        return self.current_target
    
    def check_answer(self, pressed_pin):
        """
        Check if the pressed button is correct.
        
        Args:
            pressed_pin (str): The pin that was pressed
            
        Returns:
            bool: True if correct, False if incorrect
        """
        return pressed_pin == self.current_target
    
    def increment_score(self):
        """Increment the score by 1."""
        self.score += 1
    
    def is_new_high_score(self):
        """
        Check if current score is a new high score.
        
        Returns:
            bool: True if current score is a new high score
        """
        return self.score > self.high_scores.get(self.game_mode, 0)


class GameLogic:
    """
    Main game logic controller for the NATHAN Console.
    
    This class orchestrates the game flow, handles timing,
    and coordinates between input, output, and game state.
    """
    
    def __init__(self, button_manager, led_manager, audio_manager, motor_manager, memory_manager):
        """
        Initialize the game logic controller.
        
        Args:
            button_manager: Button input manager
            led_manager: LED output manager  
            audio_manager: Audio output manager
            motor_manager: Motor output manager
            memory_manager: Memory/storage manager
        """
        self.button_manager = button_manager
        self.led_manager = led_manager
        self.audio_manager = audio_manager
        self.motor_manager = motor_manager
        self.memory_manager = memory_manager
        
        self.game_state = GameState()
        
        # Load high scores
        self._load_high_scores()
    
    def _load_high_scores(self):
        """Load high scores from memory."""
        self.game_state.high_scores = self.memory_manager.get_all_high_scores()
        print(f"High scores loaded: {self.game_state.high_scores}")
    
    def set_game_mode(self, mode):
        """
        Set the current game mode.
        
        Args:
            mode (str): The game mode to set
        """
        self.game_state.game_mode = mode
        print(f"Game mode set to: {mode}")
    
    def start_game(self):
        """Start a new game session."""
        print(f"Starting game in {self.game_state.game_mode} mode")
        print(f"Current record: {self.game_state.high_scores.get(self.game_state.game_mode, 0)}")
        
        # Play intro sequence
        self.audio_manager.play_game_intro(self.game_state.game_mode, self.button_manager)
        
        # Start the game
        self.game_state.start_game()
        
        # Game loop
        while self.game_state.is_active and not self.game_state.is_game_over:
            self._play_round()
        
        # Handle game end
        self._handle_game_end()
    
    def _play_round(self):
        """Play a single round of the game."""
        # Generate new challenge
        target_pin = self.game_state.generate_challenge()
        
        # Set game indicator
        self.led_manager.set_game_indicator(True)
        
        # Present the challenge
        self._present_challenge(target_pin)
        
        # Wait for player response
        response_pin = self._wait_for_response()
        
        # Check the response
        if response_pin:
            self._handle_response(response_pin)
        else:
            # Timeout occurred
            self._handle_timeout()
        
        # Clear game indicator
        self.led_manager.set_game_indicator(False)
    
    def _present_challenge(self, target_pin):
        """
        Present a challenge to the player.
        
        Args:
            target_pin (str): The target pin for this challenge
        """
        # Light up the target LED
        self.led_manager.turn_on_led(target_pin)
        
        # Play appropriate sound based on game mode
        if self.game_state.game_mode == "Megamix":
            # Choose random mode for Megamix
            temp_modes = ["Arcade", "Voice", "Reverse", "Light"]
            temp_mode = random.choice(temp_modes)
            
            if temp_mode != "Light":
                self.audio_manager.play_mode_sound(temp_mode, target_pin, self.button_manager)
        elif self.game_state.game_mode != "Light":
            # Play sound for non-Light modes
            self.audio_manager.play_mode_sound(self.game_state.game_mode, target_pin, self.button_manager)
    
    def _wait_for_response(self):
        """
        Wait for player response with timeout.
        
        Returns:
            str or None: The pressed pin name, or None if timeout
        """
        # Calculate timeout based on score (gets faster as score increases)
        timeout = max(0.5, 1.0 - (0.001 * self.game_state.score))
        
        start_time = time.time()
        
        while (time.time() - start_time) < timeout:
            # Check for button press
            pressed_pin = self.button_manager.get_pressed_button()
            if pressed_pin:
                # Turn off challenge LED
                self.led_manager.turn_off_led(self.game_state.current_target)
                
                # Wait for button release
                self.button_manager.wait_for_button_release(pressed_pin)
                time.sleep(0.25)  # Debounce delay
                
                return pressed_pin
            
            time.sleep(0.01)  # Small delay to prevent busy waiting
        
        # Timeout - turn off challenge LED
        self.led_manager.turn_off_led(self.game_state.current_target)
        return None
    
    def _handle_response(self, pressed_pin):
        """
        Handle player response.
        
        Args:
            pressed_pin (str): The pin that was pressed
        """
        if self.game_state.check_answer(pressed_pin):
            # Correct answer
            self._handle_correct_response(pressed_pin)
        else:
            # Wrong answer
            self._handle_wrong_response(pressed_pin)
    
    def _handle_correct_response(self, pressed_pin):
        """
        Handle a correct response.
        
        Args:
            pressed_pin (str): The correct pin that was pressed
        """
        print("Correct!")
        
        # Increment score
        self.game_state.increment_score()
        
        # Provide feedback
        self.motor_manager.vibrate_for_button(pressed_pin, True)
        self.audio_manager.play_hit_sound(pressed_pin, self.button_manager)
        
        # Check for intermission sounds
        self.audio_manager.play_intermission_sound(self.game_state.score, self.button_manager)
    
    def _handle_wrong_response(self, pressed_pin):
        """
        Handle a wrong response.
        
        Args:
            pressed_pin (str): The incorrect pin that was pressed
        """
        print(f"Wrong! Pressed {pressed_pin}, needed {self.game_state.current_target}")
        
        # Provide negative feedback
        self.motor_manager.vibrate_for_button(pressed_pin, False)
        self.audio_manager.play_fail_sounds(self.button_manager)
        
        # End the game
        self.game_state.end_game()
    
    def _handle_timeout(self):
        """Handle timeout scenario."""
        print("Game over: time expired")
        print(f"Final score: {self.game_state.score}")
        
        # End the game
        self.game_state.end_game()
    
    def _handle_game_end(self):
        """Handle end of game sequence."""
        print(f"Game ended. Final score: {self.game_state.score}")
        
        # Check for new high score
        is_new_high_score = self.game_state.is_new_high_score()
        
        if is_new_high_score:
            print(f"NEW HIGH SCORE: {self.game_state.score}")
            self.memory_manager.write_high_score(self.game_state.game_mode, self.game_state.score)
            self.game_state.high_scores[self.game_state.game_mode] = self.game_state.score
        
        # Play end game sounds
        self.audio_manager.play_game_over_sounds(is_new_high_score, self.game_state.score, self.button_manager)
        
        # Turn off all LEDs
        self.led_manager.turn_off_all_game_leds()
        
        # Reset game state
        self.game_state.reset()
    
    def get_current_score(self):
        """
        Get the current score.
        
        Returns:
            int: Current score
        """
        return self.game_state.score
    
    def get_high_score(self, mode=None):
        """
        Get high score for a specific mode or current mode.
        
        Args:
            mode (str, optional): Game mode to get score for
            
        Returns:
            int: High score for the mode
        """
        if mode is None:
            mode = self.game_state.game_mode
        return self.game_state.high_scores.get(mode, 0)