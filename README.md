
CS/EE 120B
Custom Project: Jukebox
Shane Owens
Aug, 21, 2017

Created this jukebox to play a few songs that I enjoy. It is using the ATMEGA 1284 microcontroller, 16 pin LCD display, buttons, LED's and a piezo buzzer. Current version plays "Imperial March", "PACMan Theme" and "Zelda Theme". I do not own these songs, it is just for a hobby project. 

Build:
In this project I am utilizing:
PINA as inputs from user buttons,
PINB for output to speaker and lights,
PINC and PIND for LCD display.

Components:
16 pin LCD display,
ATMEGA1284 microcontroller,
4 buttons,
2 potentiometers for volume and screen contrast (10k ohm and 1k ohm),
piezo buzzer,
7 green LEDs in smiley face pattern,
5V power supply,
jumper wires.

Introduction
My jukebox will give the user the option to choose between 3 songs. Using a button a user will be able to select a song out of 3 songs to listen to. The song will be played through a piezo buzzer and a LED light show will commence.

Components (Pin-out)
Inputs
4-Buttons(3 songs/1 stop button) using PORT A
Outputs
16-pin LCD display using PORT C and D
LEDs(7) for light display using PORT B
Piezo buzzer using PORT B

Port B  PIN B6 will control piezo buzzer and LED light show, Port A PIN’s A0...A3 will take inputs from 4 buttons. 1 button for each of the 4 songs and 1 for a stop button to discontinue the song playing.



