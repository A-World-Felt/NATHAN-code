"""
Audio Output Management Module

This module handles all audio functionality for the NATHAN Console.
It provides a clean interface for playing sounds, music, and managing audio feedback.
"""

import board
import time
import audiomixer
from audiocore import WaveFile
from audiopwmio import PWMAudioOut as AudioOut


class AudioManager:
    """
    Manages all audio output for the NATHAN Console.
    
    This class provides methods to play various sound effects, music,
    and manage the audio mixer for the game.
    """
    
    def __init__(self):
        """Initialize audio system with mixer and output."""
        # Initialize audio output
        self.audio = AudioOut(board.GP21, quiescent_value=32768)
        
        # Initialize mixer
        self.mixer = audiomixer.Mixer(
            voice_count=2, 
            sample_rate=22050, 
            channel_count=1,
            bits_per_sample=16, 
            samples_signed=True
        )
        
        # Sound file path
        self.sound_path = "/sd/sounds/"
        
        # Sound mappings for different game modes and actions
        self.hit_sounds = {
            "pin9": "5H.wav", 
            "pin10": "4H.wav",
            "pin13": "1H.wav",
            "pin14": "2H.wav",
            "pin15": "3H.wav"
        }
        
        self.mode_sounds = {
            "Arcade": {
                "mode_announce": "ModeArcade.wav",
                "pin9": "5CArcade.wav", 
                "pin10": "4CArcade.wav",
                "pin13": "1CArcade.wav",
                "pin14": "2CArcade.wav",
                "pin15": "3CArcade.wav"
            },
            "Voice": {
                "mode_announce": "ModeVoice.wav",
                "pin9": "5CVoice.wav", 
                "pin10": "4CVoice.wav",
                "pin13": "1CVoice.wav",
                "pin14": "2CVoice.wav",
                "pin15": "3CVoice.wav"
            },
            "Reverse": {
                "mode_announce": "ModeReverse.wav",
                "pin9": "5CReverse.wav", 
                "pin10": "4CReverse.wav",
                "pin13": "1CReverse.wav",
                "pin14": "2CReverse.wav",
                "pin15": "3CReverse.wav"
            },
            "Light": {
                "mode_announce": "ModeLight.wav"
            },
            "Megamix": {
                "mode_announce": "ModeMegamix.wav"
            }
        }
    
    def play_sound(self, filename, interruptible=True, button_manager=None):
        """
        Play a sound file.
        
        Args:
            filename (str): The sound file to play
            interruptible (bool): Whether the sound can be interrupted by button press
            button_manager: Optional button manager for interrupt checking
        """
        file_path = self.sound_path + filename
        print(f"Playing: {file_path}")
        
        try:
            with open(file_path, "rb") as wave_file:
                wave = WaveFile(wave_file)
                self.mixer.voice[0].play(wave)
                
                while self.mixer.voice[0].playing:
                    if interruptible and button_manager and button_manager.any_game_button_pressed():
                        break
                    time.sleep(0.01)  # Small delay to prevent busy waiting
                    
        except Exception as e:
            print(f"Error playing sound {filename}: {e}")
    
    def play_hit_sound(self, pin_name, button_manager=None):
        """
        Play the hit sound for a specific button.
        
        Args:
            pin_name (str): The pin name that was hit
            button_manager: Optional button manager for interrupt checking
        """
        if pin_name in self.hit_sounds:
            self.play_sound(self.hit_sounds[pin_name], False, button_manager)
    
    def play_mode_sound(self, mode, pin_name=None, button_manager=None):
        """
        Play a sound for a specific game mode and optionally a specific pin.
        
        Args:
            mode (str): The game mode
            pin_name (str, optional): The specific pin sound to play
            button_manager: Optional button manager for interrupt checking
        """
        if mode in self.mode_sounds:
            if pin_name and pin_name in self.mode_sounds[mode]:
                self.play_sound(self.mode_sounds[mode][pin_name], True, button_manager)
            elif "mode_announce" in self.mode_sounds[mode]:
                self.play_sound(self.mode_sounds[mode]["mode_announce"], False, button_manager)
    
    def play_menu_music(self, button_manager):
        """
        Play menu music and handle menu navigation.
        
        Args:
            button_manager: Button manager for checking input
            
        Returns:
            str: The menu action selected ("ModeDeJeu" or "DebuterPartie")
        """
        try:
            with open(self.sound_path + "MenuMusic.wav", "rb") as wave_file:
                wave = WaveFile(wave_file)
                
                while True:
                    self.mixer.voice[0].play(wave)
                    
                    while self.mixer.voice[0].playing:
                        if button_manager.bouton_pin10.value or button_manager.bouton_pin9.value:
                            return "ModeDeJeu"
                        
                        if button_manager.bouton_pin14.value:
                            return "DebuterPartie"
                        
                        time.sleep(0.01)
                        
        except Exception as e:
            print(f"Error playing menu music: {e}")
            return "DebuterPartie"  # Default action
    
    def play_score_sounds(self, score, button_manager=None):
        """
        Play score announcement sounds.
        
        Args:
            score (int): The score to announce
            button_manager: Optional button manager for interrupt checking
        """
        centaine = int(score / 100)
        dizaine = int((score - centaine * 100) / 10)
        unite = int(score - centaine * 100 - dizaine * 10)
        
        # Play hundreds
        for _ in range(centaine):
            self.play_sound("Point100.wav", True, button_manager)
        
        # Play tens
        for _ in range(dizaine):
            self.play_sound("Point10.wav", True, button_manager)
        
        # Play units
        for _ in range(unite):
            self.play_sound("Point1.wav", True, button_manager)
    
    def play_game_intro(self, mode, button_manager=None):
        """
        Play the game introduction sequence.
        
        Args:
            mode (str): The game mode being started
            button_manager: Optional button manager for interrupt checking
        """
        self.play_sound("Title.wav", True, button_manager)
        self.play_mode_sound(mode, button_manager=button_manager)
        self.play_sound("GlassMasterIntermission2.wav", True, button_manager)
        self.play_sound("HereItComes.wav", True, button_manager)
    
    def play_intermission_sound(self, score, button_manager=None):
        """
        Play intermission sounds based on score milestones.
        
        Args:
            score (int): Current score
            button_manager: Optional button manager for interrupt checking
        """
        if score == 25:
            self.play_sound("GlassMasterIntermission1.wav", True, button_manager)
        elif score == 50:
            self.play_sound("GlassMasterIntermission3.wav", True, button_manager)
        elif score == 75:
            self.play_sound("GlassMasterIntermission4.wav", True, button_manager)
        elif score == 100:
            self.play_sound("GlassMasterIntermission5.wav", True, button_manager)
    
    def play_game_over_sounds(self, is_high_score, score, button_manager=None):
        """
        Play game over sound sequence.
        
        Args:
            is_high_score (bool): Whether a new high score was achieved
            score (int): Final score
            button_manager: Optional button manager for interrupt checking
        """
        if is_high_score:
            self.play_sound("HighScore.wav", False, button_manager)
        else:
            self.play_sound("Score.wav", False, button_manager)
        
        self.play_score_sounds(score, button_manager)
    
    def play_fail_sounds(self, button_manager=None):
        """
        Play failure sound sequence.
        
        Args:
            button_manager: Optional button manager for interrupt checking
        """
        self.play_sound("Fail.wav", False, button_manager)
        self.play_sound("GlassMasterLaugh.wav", False, button_manager)
        self.play_sound("GlassMasterIntermission6.wav", False, button_manager)
    
    def start_audio(self):
        """Start the audio system."""
        self.audio.play(self.mixer)
    
    def is_playing(self):
        """
        Check if audio is currently playing.
        
        Returns:
            bool: True if audio is playing, False otherwise
        """
        return self.audio.playing