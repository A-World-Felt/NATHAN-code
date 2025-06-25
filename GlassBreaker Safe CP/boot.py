import storage
import board
import digitalio

led_raspberry25 = digitalio.DigitalInOut(board.LED)
led_raspberry25.direction = digitalio.Direction.OUTPUT
led_raspberry25.value = True