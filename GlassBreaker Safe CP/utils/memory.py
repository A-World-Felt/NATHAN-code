"""
Memory/Storage Management Module

This module handles persistent storage for game scores and settings.
It manages SD card operations and high score tracking.
"""

import storage
import microcontroller
import busio
import sdcardio
import board
import time


class MemoryManager:
    """
    Manages persistent storage for the NATHAN Console.
    
    This class handles SD card mounting, high score reading/writing,
    and other persistent storage operations.
    """
    
    def __init__(self):
        """Initialize memory management and mount SD card."""
        self.sd_mounted = False
        self.mount_sd_card()
        
        # Game modes supported
        self.game_modes = ["Arcade", "Voice", "Reverse", "Light", "Megamix"]
    
    def mount_sd_card(self):
        """
        Mount the SD card with retry logic.
        
        Attempts to mount the SD card multiple times with delays between attempts.
        """
        # SD card pin configuration
        MOSI = board.GP3
        MISO = board.GP0
        clk = board.GP2
        cs = board.GP1
        
        spi = busio.SPI(clk, MOSI=MOSI, MISO=MISO)
        
        max_attempts = 3
        delay_seconds = 2
        
        for attempt in range(1, max_attempts + 1):
            try:
                print(f"Tentative {attempt} de montage SD...")
                sd = sdcardio.SDCard(spi, cs)
                vfs = storage.VfsFat(sd)
                storage.mount(vfs, "/sd")
                print("Carte SD montée avec succès.")
                self.sd_mounted = True
                break
            except Exception as e:
                print(f"Échec de la tentative {attempt}: {e}")
                if attempt < max_attempts:
                    print(f"⏳ Nouvelle tentative dans {delay_seconds} secondes...")
                    time.sleep(delay_seconds)
                else:
                    print("Impossible de monter la carte SD après plusieurs essais.")
                    self.sd_mounted = False
    
    def read_high_score(self, game_mode):
        """
        Read the high score for a specific game mode.
        
        Args:
            game_mode (str): The game mode to read the score for
            
        Returns:
            int: The high score for the mode, or 0 if not found
        """
        if not self.sd_mounted:
            return 0
        
        filename = f"/sd/Memoire{game_mode}.txt"
        try:
            with open(filename, "r") as file:
                score = int(file.read().strip())
                return score
        except (FileNotFoundError, ValueError, OSError):
            # Create file with default score if it doesn't exist
            self.write_high_score(game_mode, 0)
            return 0
    
    def write_high_score(self, game_mode, score):
        """
        Write a new high score for a specific game mode.
        
        Only writes if the new score is higher than the existing score.
        
        Args:
            game_mode (str): The game mode to write the score for
            score (int): The new score to write
            
        Returns:
            bool: True if the score was updated, False otherwise
        """
        if not self.sd_mounted:
            return False
        
        filename = f"/sd/Memoire{game_mode}.txt"
        
        try:
            # Read current high score
            current_score = self.read_high_score(game_mode)
            
            # Only update if new score is higher
            if score > current_score:
                with open(filename, "w") as file:
                    file.write(str(score))
                return True
            
            return False
            
        except (OSError, IOError) as e:
            print(f"Error writing high score for {game_mode}: {e}")
            return False
    
    def get_all_high_scores(self):
        """
        Get high scores for all game modes.
        
        Returns:
            dict: Dictionary mapping game modes to their high scores
        """
        scores = {}
        for mode in self.game_modes:
            scores[mode] = self.read_high_score(mode)
        return scores
    
    def reset_all_scores(self):
        """Reset all high scores to 0."""
        if not self.sd_mounted:
            return False
        
        try:
            for mode in self.game_modes:
                filename = f"/sd/Memoire{mode}.txt"
                with open(filename, "w") as file:
                    file.write("0")
            return True
        except (OSError, IOError) as e:
            print(f"Error resetting scores: {e}")
            return False
    
    def is_sd_available(self):
        """
        Check if SD card is available for operations.
        
        Returns:
            bool: True if SD card is mounted and available, False otherwise
        """
        return self.sd_mounted