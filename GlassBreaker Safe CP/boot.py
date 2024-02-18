import storage
import os


os.environ[‘CIRCUITPY_FULL_BUILD’]=’1’

led_raspberry25 = digitalio.DigitalInOut(board.LED)
led_raspberry25.direction = digitalio.Direction.OUTPUT
led_raspberry25.value = 1