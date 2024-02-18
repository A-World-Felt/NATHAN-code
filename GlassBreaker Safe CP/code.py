# Write your code here :-)

import storage
import microcontroller
import busio
import sdcardio



import board
import time
import random
import digitalio
from Memoire import Memoire
from Memoire import Son




class bouton:
    #debuter_partie11 = machine.Pin(11, machine.Pin.IN, machine.Pin.PULL_DOWN)
    debuter_partie11 = digitalio.DigitalInOut(board.GP11)
    debuter_partie11.direction = digitalio.Direction.INPUT
    debuter_partie11.pull = digitalio.Pull.DOWN
    #led_raspberry25 = machine.Pin(25, machine.Pin.OUT) #led_raspberry pi
    led_raspberry25 = digitalio.DigitalInOut(board.LED)
    led_raspberry25.direction = digitalio.Direction.OUTPUT
    led_raspberry25.value = 1
    time.sleep(1)
    led_raspberry25.value=0
    #debuter_partie11.value(0)
    #bouton_pin15 = machine.Pin(15, machine.Pin.IN, machine.Pin.PULL_DOWN) #bouton pin
    bouton_pin15 = digitalio.DigitalInOut(board.GP15)
    bouton_pin15.direction = digitalio.Direction.INPUT
    bouton_pin15.pull = digitalio.Pull.DOWN
    #bouton_pin14 = machine.Pin(14, machine.Pin.IN, machine.Pin.PULL_DOWN)
    bouton_pin14 = digitalio.DigitalInOut(board.GP14)
    bouton_pin14.direction = digitalio.Direction.INPUT
    bouton_pin14.pull = digitalio.Pull.DOWN
    #bouton_pin13 = machine.Pin(13, machine.Pin.IN, machine.Pin.PULL_DOWN)
    bouton_pin13 = digitalio.DigitalInOut(board.GP13)
    bouton_pin13.direction = digitalio.Direction.INPUT
    bouton_pin13.pull = digitalio.Pull.DOWN

    bouton_pin10 = digitalio.DigitalInOut(board.GP10)
    bouton_pin10.direction = digitalio.Direction.INPUT
    bouton_pin10.pull = digitalio.Pull.DOWN

    bouton_pin9 = digitalio.DigitalInOut(board.GP9)
    bouton_pin9.direction = digitalio.Direction.INPUT
    bouton_pin9.pull = digitalio.Pull.DOWN

    #lumiere_pin15 = machine.Pin(18, machine.Pin.OUT) #bouton pin
    lumiere_pin15 = digitalio.DigitalInOut(board.GP18)
    lumiere_pin15.direction = digitalio.Direction.OUTPUT
    #lumiere_pin14 = machine.Pin(17, machine.Pin.OUT)
    lumiere_pin14 = digitalio.DigitalInOut(board.GP17)
    lumiere_pin14.direction = digitalio.Direction.OUTPUT
    #lumiere_pin13 = machine.Pin(16, machine.Pin.OUT)
    lumiere_pin13 = digitalio.DigitalInOut(board.GP16)
    lumiere_pin13.direction = digitalio.Direction.OUTPUT
    #led_pin12 = machine.Pin(12, machine.Pin.OUT) #led_pin12
    lumiere_pin12 = digitalio.DigitalInOut(board.GP12)
    lumiere_pin12.direction = digitalio.Direction.OUTPUT

    lumiere_pin10 = digitalio.DigitalInOut(board.GP19)
    lumiere_pin10.direction = digitalio.Direction.OUTPUT

    lumiere_pin9 = digitalio.DigitalInOut(board.GP20)
    lumiere_pin9.direction = digitalio.Direction.OUTPUT


    dicti = {"modeDeJeu" : "Arcade", "pin" : 0, "score" : 0, "reponse" : 0, "debut" : False, "gameover" : False, "record" : {"Arcade" : 0, "Voice" : 0, "Reverse" : 0, "Voice" : 0, "Light" : 0, "Megamix" : 0}, "lumiere" : {"pin13" : lumiere_pin13, "pin14" : lumiere_pin14, "pin15" : lumiere_pin15, "pin9": lumiere_pin9, "pin10": lumiere_pin10}}

    dicti["record"]["Arcade"] = int(Memoire.lire_memoire_txt("Arcade"))
    dicti["record"]["Voice"] = int(Memoire.lire_memoire_txt("Voice"))
    dicti["record"]["Reverse"] = int(Memoire.lire_memoire_txt("Reverse"))
    dicti["record"]["Light"] = int(Memoire.lire_memoire_txt("Light"))
    dicti["record"]["Megamix"] = int(Memoire.lire_memoire_txt("Megamix"))

    print("\n")
    print(dicti["record"])
    print("\n")

    sonHit = {"pin9": "5H.wav", "pin10": "4H.wav","pin13": "1H.wav","pin14": "2H.wav","pin15": "3H.wav"}

    SonCommande = {"Arcade" : {"SonModeDeJeu" : "ModeArcade.wav", "pin9": "5CArcade.wav", "pin10": "4CArcade.wav","pin13": "1CArcade.wav","pin14": "2CArcade.wav","pin15": "3CArcade.wav"},
    "Voice" : {"SonModeDeJeu" : "ModeVoice.wav", "pin9": "5CVoice.wav", "pin10": "4CVoice.wav","pin13": "1CVoice.wav","pin14": "2CVoice.wav","pin15": "3CVoice.wav"},
    "Reverse" : {"SonModeDeJeu" : "ModeReverse.wav", "pin9": "5CReverse.wav", "pin10": "4CReverse.wav","pin13": "1CReverse.wav","pin14": "2CReverse.wav","pin15": "3CReverse.wav"},
    "Light": {"SonModeDeJeu" : "ModeLight.wav",}, "Megamix": {"SonModeDeJeu" : "ModeMegamix.wav",}}

Son.play_sound(bouton, "GlassMasterIntro.wav")

while True:
    debuter_partie11 = bouton.debuter_partie11
    lumiere_pin12 = bouton.lumiere_pin12
    lumiere_pin13 = bouton.lumiere_pin13
    lumiere_pin14 = bouton.lumiere_pin14
    lumiere_pin15 = bouton.lumiere_pin15
    lumiere_pin10 = bouton.lumiere_pin10
    lumiere_pin9 = bouton.lumiere_pin9
    bouton_pin13 = bouton.bouton_pin13
    bouton_pin14 = bouton.bouton_pin14
    bouton_pin15 = bouton.bouton_pin15
    bouton_pin10 = bouton.bouton_pin10
    bouton_pin9 = bouton.bouton_pin9
    dicti = bouton.dicti
    if dicti["debut"] == True:
        while dicti["gameover"] == False:
            def hasard(dicti):
                #led_pin12 = machine.Pin(12, machine.Pin.OUT)
                #led_pin12.value(1) #led indiquant qu'une partie est en jeu

                lumiere_pin12.value = 1

                pin = ['pin13', 'pin14', 'pin15', 'pin10', 'pin9']
                import random
                x = random.randint(0, 4)
                dicti["reponse"] = pin[x]


            def timer(dicti):
                ModeDeJeuEphemere = 0
                sec = 1

                time.sleep(1)
                print(dicti["reponse"])

                if dicti["modeDeJeu"] == "Megamix":
                    ModeDeJeu = ["Arcade", "Voice", "Reverse", "Light"]
                    j = random.randint(0, 3)
                    ModeDeJeuEphemere = ModeDeJeu[j]

                    no_pin = dicti["lumiere"][dicti["reponse"]]
                    no_pin.value = 1

                    if ModeDeJeuEphemere != "Light":
                            Son.play_sound(bouton, bouton.SonCommande[ModeDeJeuEphemere][dicti["reponse"]])



                no_pin = dicti["lumiere"][dicti["reponse"]]
                no_pin.value = 1

                if dicti["modeDeJeu"] != "Light" and dicti["modeDeJeu"] != "Megamix":
                        Son.play_sound(bouton, bouton.SonCommande[dicti["modeDeJeu"]][dicti["reponse"]])







                while sec > 0:
                    if bouton_pin15.value == 1 or bouton_pin14.value == 1 or bouton_pin13.value == 1 or bouton_pin9.value == 1 or bouton_pin10.value == 1:
                        if bouton_pin15.value == 1:
                            lumiere_pin12.value = 0 # 1 pour que la lumiere s'allume)

                            print('x')
                            time.sleep(0.25)
                            dicti["pin"] = "pin15"
                            no_pin.value = 0
                            break


                        if bouton_pin14.value == 1:
                            lumiere_pin12.value=0 # 1 pour que la lumiere s'allume)

                            print('x')
                            time.sleep(0.25)
                            dicti["pin"] = "pin14"
                            no_pin.value = 0
                            break


                        if bouton_pin13.value == 1:
                            lumiere_pin12.value=0 # 1 pour que la lumiere s'allume)

                            print('x')
                            time.sleep(0.25)
                            dicti["pin"] = "pin13"
                            no_pin.value = 0
                            break

                        if bouton_pin10.value == 1:
                            lumiere_pin12.value = 0 # 1 pour que la lumiere s'allume)

                            print('x')
                            time.sleep(0.25)
                            dicti["pin"] = "pin10"
                            no_pin.value = 0
                            break

                        if bouton_pin9.value == 1:
                            lumiere_pin12.value = 0 # 1 pour que la lumiere s'allume)

                            print('x')
                            time.sleep(0.25)
                            dicti["pin"] = "pin9"
                            no_pin.value = 0
                            break

                    else:

                        #print(sec)
                        time.sleep(0.01)
                        sec -= (0.01 + (0.001*int(dicti["score"])))


                lumiere_pin12.value=0 # 1 pour que la lumiere s'allume)

                if sec <= 0:
                    print('game over : temps ecoule')
                    print(f"score : {dicti["score"]}")
                    dicti["gameover"] = True
                    dicti["debut"] = False
                    if dicti["score"] > dicti["record"][dicti["modeDeJeu"]]:
                        dicti["record"][dicti["modeDeJeu"]] = dicti["score"]
                        Memoire.ecrire_memoire_txt(dicti["modeDeJeu"], dicti["record"][dicti["modeDeJeu"]])
                        Son.play_sound(bouton, "HighScore.wav")
                        print(f"NOUVEAU RECORD : {dicti["record"][dicti["modeDeJeu"]]}")
                    else:
                        Son.play_sound(bouton, "Score.wav")
                    Son.play_soundScore(bouton, dicti["score"])


                    dicti["score"] = 0
                    led_off()


            def correction(dicti):
                if dicti["reponse"] == dicti["pin"]:
                    print('correct')
                    Son.play_sound(bouton, f"{bouton.sonHit[dicti["pin"]]}")
                    dicti["score"] += 1
                    if dicti["score"]==25:
                        Son.play_sound(bouton, "GlassMasterIntermission1.wav")
                    if dicti["score"]==50:
                        Son.play_sound(bouton, "GlassMasterIntermission3.wav")
                    if dicti["score"]==75:
                        Son.play_sound(bouton, "GlassMasterIntermission4.wav")
                    if dicti["score"]==100:
                        Son.play_sound(bouton, "GlassMasterIntermission5.wav")
                    dicti["pin"]=0
                else:
                    if dicti["gameover"] != True:
                        Son.play_sound(bouton, "Fail.wav")
                        Son.play_sound(bouton, "GlassMasterLaugh.wav")
                        Son.play_sound(bouton, "GlassMasterIntermission6.wav")
                        time.sleep(0.25)
                        print('game over : mauvais bouton')
                        print(dicti["pin"], dicti["reponse"])
                        print(f"score : {dicti["score"]}")
                        if dicti["score"] > dicti["record"][dicti["modeDeJeu"]]:
                            dicti["record"][dicti["modeDeJeu"]] = dicti["score"]
                            print(f"NOUVEAU RECORD : {dicti["record"][dicti["modeDeJeu"]]}")
                            Son.play_sound(bouton, "HighScore.wav")
                            Memoire.ecrire_memoire_txt(dicti["modeDeJeu"], dicti["record"][dicti["modeDeJeu"]])
                        else:
                            Son.play_sound(bouton, "Score.wav")
                        Son.play_soundScore(bouton, dicti["score"])

                        dicti["gameover"] = True
                        dicti["debut"] = False

                        dicti["score"] = 0
                    led_off()

            def led_off():

                lumiere_pin13.value=0
                lumiere_pin14.value=0
                lumiere_pin15.value=0
                lumiere_pin10.value=0
                lumiere_pin9.value=0


            hasard(dicti)
            timer(dicti)
            correction(dicti)


    def ModeDeJeu(bouton, choix):
        i=0
        modeDeJeu = ["Arcade", "Voice", "Reverse", "Light", "Megamix"]
        while True:


            if bouton.bouton_pin13.value:
                return modeDeJeu[i]
            if bouton.bouton_pin14.value:
                i-=1
                if i<=0:
                    i=4
                Son.play_soundModeDeJeu(bouton, bouton.SonCommande[modeDeJeu[i]]["SonModeDeJeu"])



            if bouton.bouton_pin9.value:
                i+=1
                if i>=5:
                    i=0
                Son.play_soundModeDeJeu(bouton, bouton.SonCommande[modeDeJeu[i]]["SonModeDeJeu"])







    def debuter_partie(dicti):
        arret = False

        print(f"record : {dicti["record"][dicti["modeDeJeu"]]}")

        Son.play_sound(bouton, "Title.wav")
        Son.play_sound(bouton, bouton.SonCommande[dicti["modeDeJeu"]]["SonModeDeJeu"])
        Son.play_sound(bouton, "GlassMasterIntermission2.wav")
        Son.play_sound(bouton, "HereItComes.wav")
        print("%%%")
        dicti["debut"] = True
        arret = True
        time.sleep(0.25)
        depart = False


    def Menu(bouton):
        while True:
            choix = Son.play_soundMenu(bouton, bouton.SonCommande)

            if choix == "ModeDeJeu":
                dicti["modeDeJeu"] = ModeDeJeu(bouton, choix)
                print(f"Mode de Jeu = {dicti["modeDeJeu"]}")

            if choix == "DebuterPartie":
                time.sleep(1)
                debuter_partie(bouton.dicti)
                return

    if dicti["debut"] == False:
        if dicti["gameover"] == True:
            dicti["gameover"] = False


    Menu(bouton)



