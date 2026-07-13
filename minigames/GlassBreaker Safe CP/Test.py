import storage
import microcontroller
import busio
import sdcardio
import time
from Memoire import Son


class Test:

    def runTest(bouton, dicti, mixer):
        #Son.play_sound(bouton, "1point", "/sounds/")
        while True:
            if bouton.bouton_pin15.value :
                Son.play_sound(bouton, f"{bouton.sonHit["pin15"]}", mixer)
                print("pin15")
                no_pin = dicti["lumiere"]["pin15"]
                no_pin.value = 1
                time.sleep(0.25)
                no_pin.value = 0


            if bouton.bouton_pin14.value :
                Son.play_sound(bouton, f"{bouton.sonHit["pin14"]}", mixer)
                print("pin14")
                no_pin = dicti["lumiere"]["pin14"]
                no_pin.value = 1
                time.sleep(0.25)
                no_pin.value = 0

            if bouton.bouton_pin13.value :
                Son.play_sound(bouton, f"{bouton.sonHit["pin13"]}", mixer)
                print("pin13")
                no_pin = dicti["lumiere"]["pin13"]
                no_pin.value = 1
                time.sleep(0.25)
                no_pin.value = 0

            if bouton.bouton_pin10.value :
                Son.play_sound(bouton, f"{bouton.sonHit["pin10"]}", mixer)
                print("pin10")
                no_pin = dicti["lumiere"]["pin10"]
                no_pin.value = 1
                time.sleep(0.25)
                no_pin.value = 0

            if bouton.bouton_pin9.value :
                Son.play_sound(bouton, f"{bouton.sonHit["pin9"]}", mixer)
                print("pin9")
                no_pin = dicti["lumiere"]["pin9"]
                no_pin.value = 1
                time.sleep(0.25)
                no_pin.value = 0






        return

# Ecrit ton
