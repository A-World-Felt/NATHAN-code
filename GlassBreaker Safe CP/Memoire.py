
import storage
import microcontroller
import busio
import sdcardio
import board
import time
import os

MOSI = board.GP3
MISO = board.GP0
clk = board.GP2
cs = board.GP1

spi = busio.SPI(clk, MOSI=MOSI, MISO=MISO)

if spi.try_lock():
    print("SPI is available")
    spi.unlock()

time.sleep(1)  # Petit délai avant l'initialisation


try:
    sd = sdcardio.SDCard(spi, cs)
    print("SD Card initialized successfully")
except Exception as e:
    print("Failed to initialize SD Card:", e)

try:
    vfs = storage.VfsFat(sd)
    storage.mount(vfs, '/sd')
    print("SD Card mounted successfully at /sd")
except Exception as e:
    print("Failed to mount SD Card:", e)





class Memoire:
    
    def print_memoire():
        for fichier in os.listdir("/sd"):
            print(fichier)

    def reset_memoire():
        with open("/sd/MemoireArcade.txt", 'w') as Memoire_txt:
            Memoire_txt.write("0")
            Memoire_txt.close

        with open("/sd/MemoireVoice.txt", 'w') as Memoire_txt:
            Memoire_txt.write("0")
            Memoire_txt.close

        with open("/sd/MemoireReverse.txt", 'w') as Memoire_txt:
            Memoire_txt.write("0")
            Memoire_txt.close

        with open("/sd/MemoireLight.txt", 'w') as Memoire_txt:
            Memoire_txt.write("0")
            Memoire_txt.close

        with open("/sd/MemoireMegamix.txt", 'w') as Memoire_txt:
            Memoire_txt.write("0")
            Memoire_txt.close




    def ecrire_memoire_txt(ModeDeJeu, record=0):

        if ModeDeJeu == "Arcade":
            with open("/sd/MemoireArcade.txt", "r") as Memoire_txt:
                record_memoire = int(Memoire_txt.read())
                #print(f"{record_memoire} record memoire 1")
                Memoire_txt.close()
                #print("***")
                #print(f"{record} rec")

                if record > record_memoire:
                    #print("&&&")
                    with open("/sd/MemoireArcade.txt", "w") as Memoire_txt2:

                        Memoire_txt2.write(f"{record}")
                        Memoire_txt2.close

        if ModeDeJeu == "Voice":
            with open("/sd/MemoireVoice.txt", "r") as Memoire_txt:
                record_memoire = int(Memoire_txt.read())
                #print(f"{record_memoire} record memoire 1")
                Memoire_txt.close()
                #print("***")
                #print(f"{record} rec")

                if record > record_memoire:
                    #print("&&&")
                    with open("/sd/MemoireVoice.txt", "w") as Memoire_txt2:

                        Memoire_txt2.write(f"{record}")
                        Memoire_txt2.close

        if ModeDeJeu == "Reverse":
            with open("/sd/MemoireReverse.txt", "r") as Memoire_txt:
                record_memoire = int(Memoire_txt.read())
                #print(f"{record_memoire} record memoire 1")
                Memoire_txt.close()
                #print("***")
                #print(f"{record} rec")

                if record > record_memoire:
                    #print("&&&")
                    with open("/sd/MemoireReverse.txt", "w") as Memoire_txt2:

                        Memoire_txt2.write(f"{record}")
                        Memoire_txt2.close

        if ModeDeJeu == "Light":
            with open("/sd/MemoireLight.txt", "r") as Memoire_txt:
                record_memoire = int(Memoire_txt.read())
                #print(f"{record_memoire} record memoire 1")
                Memoire_txt.close()
                #print("***")
                #print(f"{record} rec")

                if record > record_memoire:
                    #print("&&&")
                    with open("/sd/MemoireLight.txt", "w") as Memoire_txt2:

                        Memoire_txt2.write(f"{record}")
                        Memoire_txt2.close

        if ModeDeJeu == "Megamix":
            with open("/sd/MemoireMegamix.txt", "r") as Memoire_txt:
                record_memoire = int(Memoire_txt.read())
                #print(f"{record_memoire} record memoire 1")
                Memoire_txt.close()
                #print("***")
                #print(f"{record} rec")

                if record > record_memoire:
                    #print("&&&")
                    with open("/sd/MemoireMegamix.txt", "w") as Memoire_txt2:

                        Memoire_txt2.write(f"{record}")
                        Memoire_txt2.close




    def lire_memoire_txt(ModeDeJeu):
        
        Memoire.print_memoire()
        if ModeDeJeu == "Arcade":
            with open("/sd/MemoireArcade.txt", "r") as Memoire_txt:

                record_memoire = Memoire_txt.read()
                Memoire_txt.close()
                return record_memoire
                #print(f"{record_memoire} memoire lu")

        if ModeDeJeu == "Voice":
            with open("/sd/MemoireVoice.txt", "r") as Memoire_txt:

                record_memoire = Memoire_txt.read()
                Memoire_txt.close()
                return record_memoire
                #print(f"{record_memoire} memoire lu")

        if ModeDeJeu == "Reverse":
            with open("/sd/MemoireReverse.txt", "r") as Memoire_txt:

                record_memoire = Memoire_txt.read()
                Memoire_txt.close()
                return record_memoire
                #print(f"{record_memoire} memoire lu")

        if ModeDeJeu == "Light":
            with open("/sd/MemoireLight.txt", "r") as Memoire_txt:

                record_memoire = Memoire_txt.read()
                Memoire_txt.close()
                return record_memoire
                #print(f"{record_memoire} memoire lu")

        if ModeDeJeu == "Megamix":
            with open("/sd/MemoireMegamix.txt", "r") as Memoire_txt:

                record_memoire = Memoire_txt.read()
                Memoire_txt.close()
                return record_memoire
                #print(f"{record_memoire} memoire lu")



from audiocore import WaveFile
from audiopwmio import PWMAudioOut as AudioOut
import digitalio
import audiopwmio

audio = AudioOut(board.GP21)



class Son:

    def popDelete():

        audio
        for i in range(0,10):
            audio.volume = (1-(i/10))
            time.sleep(0.1)

    def afficherSon():
        import os
        print(os.listdir("/sd/sounds"))


    def play_sound(bouton, son, path = "/sd/sounds/" ):
        print(path+son)
        with open(path+son, "rb") as wave_file:
            wave = WaveFile(wave_file)
            audio.play(wave)
            while audio.playing:
                if bouton.bouton_pin15.value or bouton.bouton_pin14.value or bouton.bouton_pin13.value or bouton.bouton_pin10.value or bouton.bouton_pin9.value:
                    break
                pass # code pendant que le son jou
        wave_file.close()

    def play_soundModeDeJeu(bouton, son, path = "/sd/sounds/"):
        print(path+son)
        with open(path+son, "rb") as wave_file:
            wave = WaveFile(wave_file)
            audio.play(wave)
            while audio.playing:
                pass # code pendant que le son jou
        #Son.popDelete()
        wave_file.close()

    def play_soundMenu(bouton,SonCommande, path = "/sd/sounds/"):

        with open(path+"MenuMusic.wav", "rb") as wave_file:
            wave = WaveFile(wave_file)
            while True:
                audio.play(wave)
                while audio.playing:
                    if bouton.bouton_pin14.value:
                        wave_file.close()
                        #time.sleep(0.5)
                        return "ModeDeJeu"

                    if bouton.bouton_pin13.value:
                        wave_file.close()
                        #time.sleep(0.5)
                        return "DebuterPartie"
                    pass # code pendant que le son jou
        wave_file.close()

    def play_soundScore(bouton, score):
        centaine = 0
        dizaine = 0
        unite = 0

        centaine = int(score/100)
        dizaine = int((score-centaine*100)/10)
        unite = int((score-centaine*100-dizaine*10))

        for i in range(centaine):
            Son.play_sound(bouton, "Point100.wav")

        for i in range(dizaine):
            Son.play_sound(bouton, "Point10.wav")

        for i in range(unite):
            Son.play_sound(bouton, "Point1.wav")


#Memoire.ecrire_memoire_txt(3)
#utime.sleep(5)
#Memoire.lire_memoire_txt()
#Memoire.lire_memoire_txt()
#Memoire.reset_memoire()
#afficherSon()
