#!/usr/bin/python3

######################
# Sound FX Server for DMG Tribble
# just plays two audio files depending on state: screaming, or purring.
# Nadine Lessio
# June2020
# port: /dev/cu.usbmodem1421
######################

## TODO: Better Audio Control. ie: can cut off or interrupt. 
## ISSUE: Sound doesn't keep playing when you hold it. 

import serial
import simpleaudio as sa
import threading
import time


is_purring = False
is_screaming = False

sound_purr = '85410__readeonly__bobbiepeaches04.wav'
sound_scream = '432735__xpoki__alienbabycry.wav'
wave_obj_purr = sa.WaveObject.from_wave_file(sound_purr)
wave_obj_scream = sa.WaveObject.from_wave_file(sound_scream)

port = '/dev/cu.usbmodem1421'
baud = 9600
serial_port = serial.Serial(port, baud, timeout=5)
print(serial_port)

is_purring = False
is_screaming = False


def playSounds():
	while True:
		if is_purring:
			#print("purring")
			play_obj_purr = wave_obj_purr.play()
			play_obj_purr.wait_done()  
		elif is_screaming:
			#print("screaming")
			play_obj_scream = wave_obj_scream.play()
			play_obj_scream.wait_done()  
		else:
			time.sleep(1)


def handleData(state):
	global is_screaming
	global is_purring

	if state == "purring":
		print("purr purr")
		is_purring = True
		is_screaming = False
	elif state == "screaming":
		is_purring = False
		is_screaming = True
		print("scream!")
	else:
		is_purring = False
		is_screaming = False



def readFromPort():
	if serial_port:
		while True:
			data = serial_port.readline().strip().decode()
			handleData(data)
			#print(data)
			#playSounds(data)

if __name__ == "__main__":
	thread = threading.Thread(target=playSounds)
	thread.start()

	readFromPort()

