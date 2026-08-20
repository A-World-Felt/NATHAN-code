"""
NATHAN Console - Main Entry Point

This is the main entry point for the NATHAN Console game system.
It initializes all modules and orchestrates the main game loop.

The NATHAN Console is a button-based reaction game with multiple game modes,
LED feedback, audio cues, and haptic feedback through vibration motors.

Author: NATHAN Console Development Team
Version: 2.0 (Refactored Architecture)
"""

import time
from input.buttons import ButtonManager
from output.leds import LEDManager
from output.audio import AudioManager
from output.motors import MotorManager
from utils.memory import MemoryManager
from game.logic import GameLogic
from game.menu import MenuSystem


class NathanConsole:
    """
    Main NATHAN Console application class.
    
    This class coordinates all the different modules and manages
    the overall application flow.
    """
    
    def __init__(self):
        """Initialize the NATHAN Console system."""
        print("Initializing NATHAN Console...")
        
        # Initialize all managers
        self.button_manager = ButtonManager()
        self.led_manager = LEDManager()
        self.audio_manager = AudioManager()
        self.motor_manager = MotorManager()
        self.memory_manager = MemoryManager()
        
        # Initialize game logic
        self.game_logic = GameLogic(
            self.button_manager,
            self.led_manager, 
            self.audio_manager,
            self.motor_manager,
            self.memory_manager
        )
        
        # Initialize menu system
        self.menu_system = MenuSystem(
            self.button_manager,
            self.audio_manager,
            self.game_logic
        )
        
        # Set default game mode to Arcade
        self.menu_system.set_default_mode("Arcade")
        
        print("NATHAN Console initialized successfully!")
    
    def run_startup_sequence(self):
        """Run the startup sequence with visual and audio feedback."""
        print("Running startup sequence...")
        
        # LED startup pattern
        self.led_manager.execute_sequence_pattern()
        
        # Start audio system
        self.audio_manager.start_audio()
        
        # Play intro sound
        self.audio_manager.play_sound("GlassMasterIntro.wav", False)
        
        print("Startup sequence complete!")
    
    def run_main_loop(self):
        """
        Run the main application loop.
        
        This method contains the main game loop that handles menu display,
        game execution, and system management.
        """
        print("Starting main game loop...")
        
        while True:
            try:
                # Ensure audio is playing
                if not self.audio_manager.is_playing():
                    self.audio_manager.start_audio()
                
                # Check if a game is currently active
                if self.game_logic.game_state.is_active:
                    # Game is running - this should be handled by game logic
                    continue
                
                # Show startup pattern
                self.led_manager.execute_sequence_pattern()
                
                # Show main menu and get user choice
                action = self.menu_system.show_main_menu()
                
                if action == "start_game":
                    # Start the selected game
                    self.start_game()
                
                # Small delay before next iteration
                time.sleep(0.1)
                
            except KeyboardInterrupt:
                print("\\nShutdown requested by user")
                break
            except Exception as e:
                print(f"Error in main loop: {e}")
                # Try to recover by resetting game state
                self.game_logic.game_state.reset()
                time.sleep(1)
    
    def start_game(self):
        """Start a game session."""
        try:
            print(f"Starting game in {self.menu_system.get_current_mode()} mode")
            self.game_logic.start_game()
        except Exception as e:
            print(f"Error during game: {e}")
            # Reset game state on error
            self.game_logic.game_state.reset()
    
    def shutdown(self):
        """Gracefully shutdown the NATHAN Console."""
        print("Shutting down NATHAN Console...")
        
        # Turn off all outputs
        self.led_manager.turn_off_all_game_leds()
        self.led_manager.set_game_indicator(False)
        self.motor_manager.stop_all_motors()
        
        print("NATHAN Console shutdown complete.")


def run_tests():
    """
    Run system tests if test mode is enabled.
    
    This function provides a way to test all hardware components
    systematically during development or troubleshooting.
    """
    print("Running system tests...")
    
    # Initialize managers for testing
    button_manager = ButtonManager()
    led_manager = LEDManager()
    audio_manager = AudioManager()
    motor_manager = MotorManager()
    
    print("Testing LEDs...")
    led_manager.execute_sequence_pattern()
    
    print("Testing motors...")
    motor_manager.vibrate_left(0.5)
    time.sleep(0.2)
    motor_manager.vibrate_right(0.5)
    
    print("Testing audio...")
    audio_manager.start_audio()
    
    print("Testing buttons (press any game button to continue)...")
    while not button_manager.any_game_button_pressed():
        pressed = button_manager.get_pressed_button()
        if pressed:
            print(f"Button {pressed} pressed!")
            led_manager.turn_on_led(pressed)
            audio_manager.play_hit_sound(pressed)
            motor_manager.vibrate_for_button(pressed)
            time.sleep(0.5)
            led_manager.turn_off_led(pressed)
            break
        time.sleep(0.1)
    
    print("System tests complete!")


def main():
    """
    Main entry point for the NATHAN Console application.
    
    This function is called when the script is run directly.
    It handles initialization, startup, and the main game loop.
    """
    print("=" * 50)
    print("NATHAN Console Starting Up")
    print("Version 2.0 - Modular Architecture")
    print("=" * 50)
    
    # Check if we're in test mode
    # This could be set via a configuration file or environment variable
    TEST_MODE = False  # Set to True to run hardware tests
    
    if TEST_MODE:
        run_tests()
        return
    
    try:
        # Create and initialize the console
        console = NathanConsole()
        
        # Run startup sequence
        console.run_startup_sequence()
        
        # Start main application loop
        console.run_main_loop()
        
    except KeyboardInterrupt:
        print("\\nApplication interrupted by user")
    except Exception as e:
        print(f"Fatal error: {e}")
    finally:
        # Ensure clean shutdown
        try:
            console.shutdown()
        except:
            print("Emergency shutdown")


# Module-level configuration
__version__ = "2.0.0"
__author__ = "NATHAN Console Development Team"

# Only run main() if this script is executed directly
if __name__ == "__main__":
    main()