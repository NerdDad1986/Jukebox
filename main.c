/*	Partner(s) Name & E-mail:Shane Owens sowen003@ucr.edu 861197209
 *	Assignment: Custom Lab Assignment
 *	Exercise Description: Jukebox that will play Star wars, PACMAN and Zelda songs on a piezo buzzer.
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h> // main io
#include <avr/interrupt.h> //include interrupts
#include "io.c" //included from directory

#define PAC_B4 493.88//These are all my defined tones, I looked for a tone directory but didnt find one for atmel studio, but this works fine. it is alot of code.
#define PAC_B5 987.77
#define PAC_G5f 739.99
#define PAC_E5f 622.25
#define PAC_F5 698.46
#define PAC_E5 659.25
#define PAC_C5 523.25
#define PAC_C6 1046.50
#define PAC_A5 880.00
#define PAC_G5 783.99
#define PAC_A5f 830.61
#define zelda_B0  31
#define zelda_C1  33
#define zelda_CS1 35
#define zelda_D1  37
#define zelda_DS1 39
#define zelda_E1  41
#define zelda_F1  44
#define zelda_FS1 46
#define zelda_G1  49
#define zelda_GS1 52
#define zelda_A1  55
#define zelda_AS1 58
#define zelda_B1  62
#define zelda_C2  65
#define zelda_CS2 69
#define zelda_D2  73
#define zelda_DS2 78
#define zelda_E2  82
#define zelda_F2  87
#define zelda_FS2 93
#define zelda_G2  98
#define zelda_GS2 104
#define zelda_A2  110
#define zelda_AS2 117
#define zelda_B2  123
#define zelda_C3  131
#define zelda_CS3 139
#define zelda_D3  147
#define zelda_DS3 156
#define zelda_E3  165
#define zelda_F3  175
#define zelda_FS3 185
#define zelda_G3  196
#define zelda_GS3 208
#define zelda_A3  220
#define zelda_AS3 233
#define zelda_B3  247
#define zelda_C4  262
#define zelda_CS4 277
#define zelda_D4  294
#define zelda_DS4 311
#define zelda_E4  330
#define zelda_F4  349
#define zelda_FS4 370
#define zelda_G4  392
#define zelda_GS4 415
#define zelda_A4  440
#define zelda_AS4 466
#define zelda_B4  494
#define zelda_C5  523
#define zelda_CS5 554
#define zelda_D5  587
#define zelda_DS5 622
#define zelda_E5  659
#define zelda_F5  698
#define zelda_FS5 740
#define zelda_G5  784
#define zelda_GS5 831
#define zelda_A5  880
#define zelda_AS5 932
#define zelda_B5  988
#define zelda_C6  1047
#define zelda_CS6 1109
#define zelda_D6  1175
#define zelda_DS6 1245
#define zelda_E6  1319
#define zelda_F6  1397
#define zelda_FS6 1480
#define zelda_G6  1568
#define zelda_GS6 1661
#define zelda_A6  1760
#define zelda_AS6 1865
#define zelda_B6  1976
#define zelda_C7  2093
#define zelda_CS7 2217
#define zelda_D7  2349
#define zelda_DS7 2489
#define zelda_E7  2637
#define zelda_F7  2794
#define zelda_FS7 2960
#define zelda_G7  3136
#define zelda_GS7 3322
#define zelda_A7  3520
#define zelda_AS7 3729
#define zelda_B7  3951
#define zelda_C8  4186
#define zelda_CS8 4435
#define zelda_D8  4699
#define zelda_DS8 4978
#define SW_C 261
#define SW_D 294
#define SW_E 329
#define SW_F 349
#define SW_G 391
#define SW_GS 415
#define SW_A 440
#define SW_AS 455
#define SW_B 466
#define SW_CH 523
#define SW_CSH 554
#define SW_DH 587
#define SW_DSH 622
#define SW_EH 659
#define SW_FH 698
#define SW_FSH 740
#define SW_GH 784
#define SW_GSH 830
#define SW_AH 880

volatile unsigned char TimerFlag = 0; //declaration of Timerflag variable
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

unsigned char buttonPAC; //variables for PACMAN song
unsigned char notesIncPAC;
unsigned char lengthIncPAC;
unsigned char numNotesPAC = 39;

unsigned char buttonZelda; //variables for Zelda song
unsigned char notesIncZelda;
unsigned char lengthIncZelda;
unsigned char numNotesZelda = 43;

unsigned char buttonSW; //variables for Star Wars Son "Imperial March"
unsigned char notesIncSW;
unsigned char lengthIncSW;
unsigned char numNotesSW = 37;

unsigned char buttonStop; //variable for stop button

unsigned char i = 0; //variables for the counters within the song code
unsigned char j = 0;
unsigned char k = 0;

enum PACstates {Start0, Init0, Ready0, Play0} PACstate; //enum for state machines
enum Zeldastates {Start1, Init1, Ready1, Play1} Zeldastate;
enum SWstates {Start2, Init2, Ready2, Play2} SWstate;
enum Stopstates {Start3, Init3, Stop1} Stopstate;


double notesPAC[] = //notes for pacman song
{
	0.00, PAC_B4, PAC_B5, PAC_G5f, PAC_E5f, PAC_B5, PAC_F5, PAC_E5, 0.00, PAC_C5, 
	PAC_C6, PAC_A5, PAC_E5, PAC_C6, PAC_A5, PAC_E5, 0.00, PAC_B4, PAC_B5, PAC_G5f, 
	PAC_E5f, PAC_B5, PAC_F5, PAC_E5, 0.00, PAC_E5, 0.00, PAC_E5, PAC_F5, 0.00, 
	PAC_F5, PAC_G5f, PAC_G5, 0.00, PAC_G5, PAC_A5f, PAC_A5, PAC_B5, 0.00
	
}; //39 COUNT

double lengthPAC[] = //length of notes played in pacman song
{
	10, 12, 12, 12, 12, 6, 14, 24, 3, 12, 
	12, 12, 12, 6, 14, 24, 3, 12, 12, 12, 
	12, 6, 14, 24, 3, 6, 1, 6, 12, 3, 
	6, 6, 12, 3, 6, 6, 12, 24, 1
	
}; //39 COUNT

double notesZelda[] = //notes for zelda song
{
	zelda_D5, zelda_F5, zelda_D6, zelda_D5, zelda_F5, zelda_D6, zelda_D6, zelda_E6, zelda_F6, zelda_E6, 
	zelda_F6, zelda_E6, zelda_C6, zelda_A5, zelda_A5,zelda_D5, zelda_F5, zelda_G5, zelda_A5, zelda_A5, 
	zelda_D5, zelda_F5, zelda_G5, zelda_E5, zelda_D5, zelda_F5, zelda_D6, zelda_D5, zelda_F5, zelda_D6, 
	zelda_E6, zelda_F6, zelda_E6, zelda_C6, zelda_A5, zelda_A5, zelda_D5, zelda_F5, zelda_G5, zelda_A5, 
	zelda_A5, zelda_D5, 0.00
	
}; //43 COUNT

double lengthZelda[] = // length of notes played in zelda song
{
	10, 10, 20, 10, 10, 20, 20, 10, 10, 10, 
	10, 10, 10, 20, 20, 10, 10, 10, 20, 20, 
	10, 10, 10, 10, 10, 20, 10, 10, 20, 20, 
	10, 10, 10, 10, 10, 20, 20, 10, 10, 30, 
	20, 30, 1

}; //COUNT 43

double notesSW[] = //notes for Star Wars song
{
	SW_A, SW_A, SW_A, SW_F, SW_CH, SW_A, SW_F, SW_CH, SW_A, SW_EH, 
	SW_EH, SW_EH, SW_FH, SW_CH, SW_GS, SW_F, SW_CH, SW_A, 0.00, SW_AH, 
	SW_A, SW_A, SW_AH, SW_GSH, SW_GH, SW_FSH, SW_FH, SW_FSH, 0.00, SW_AS, 
	SW_DSH, SW_DH, SW_CSH, SW_CH, SW_B, SW_CH, 0.00

}; //37 COUNT
	
double lengthSW[] = //length of notes played for star wars song
{
	50, 50, 50, 35, 15, 50, 35, 15, 65, 50,
	50, 50, 50, 35, 15, 50, 35, 15, 50, 35, 
	15, 65, 50, 50, 30, 15, 50, 32.5, 17.5, 15, 
	15, 25, 32.5, 25, 50, 32.5, 1
	
}; //37 COUNT
		
void TickPAC() //Tick function for PACMAN
{
	buttonPAC = ~PINA & 0x01;
	buttonStop =~PINA & 0x08;
	switch(PACstate)
	{
		case Start0:
		PACstate = Init0;
		break;
		
		case Init0:
		i = 0;
		notesIncPAC = 0;
		lengthIncPAC = 0;	
		if(!(buttonPAC))
		{
			PACstate = Init0;
		}
		else
		{
			PACstate = Ready0;
		}	
		break;
		
		case Ready0:
		if(buttonPAC)
		{
			PACstate = Ready0;
		}
		else
		{
			PACstate = Play0;
		}
		break;
		
		
		case Play0:
		if(i < lengthPAC[lengthIncPAC] && (!buttonStop))
		{
			i++;
		}
		else
		{
			i = 0;
			notesIncPAC++;
			lengthIncPAC++;
			if(notesIncPAC == numNotesPAC||buttonStop)
			{
				PACstate = Init0;
			}
			else
			{
				set_PWM(notesPAC[notesIncPAC]);
			}
		}
		break;
		
		default:
		PACstate = Start0;
		break;
	}
}
void TickZelda() //Tick function for Zelda song
{
	buttonZelda = ~PINA & 0x02;
		buttonStop =~PINA & 0x08;

	switch(Zeldastate)
	{
		case Start1:
		Zeldastate = Init1;
		break;
		
		case Init1:
		j = 0;
		notesIncZelda = 0;
		lengthIncZelda = 0;
		if(!(buttonZelda))
		{
			Zeldastate = Init1;
		}
		else
		{
			Zeldastate = Ready1;
		}
		break;
		
		
		case Ready1:
		if(buttonZelda)
		{
			Zeldastate = Ready1;
		}
		else
		{
			Zeldastate = Play1;
		
		}
		break;
		
		
		case Play1:
		if(j < lengthZelda[lengthIncZelda])
		{
			j++;
		}
		else
		{
			j = 0;
			notesIncZelda++;
			lengthIncZelda++;
			if(notesIncZelda == numNotesZelda)
			{
				Zeldastate = Init1;
				buttonZelda = ~PINA & 0x00;
			}
			else
			{
				set_PWM(notesZelda[notesIncZelda]);
			}
		}
		break;
		
		default:
		Zeldastate = Start1;
		break;
	}
}
void TickSW() //Tick function for Star Wars song
{
	buttonSW = ~PINA & 0x04;

	switch(SWstate)
	{
		case Start2:
		SWstate = Init2;
		break;
		
		
		case Init2:
		k = 0;
		notesIncSW = 0;
		lengthIncSW = 0;
		if(!(buttonSW))
		{
			SWstate = Init2;
		}
		else
		{
			SWstate = Ready2;
		}
		break;
		
		
		case Ready2:
		if(buttonSW)
		{
			SWstate = Ready2;
		}
		else
		{
			SWstate = Play2;
		}
		break;
		
		
		case Play2:
		if(k < lengthSW[lengthIncSW])
		{
			k++;
		}
		else
		{
			k = 0;
			notesIncSW++;
			lengthIncSW++;
			if(notesIncSW == numNotesSW)
			{
				SWstate = Init2;
			}
			else
			{
				set_PWM(notesSW[notesIncSW]);
			}
		}
		break;
		
		default:
		SWstate = Start2;
		break;
	}
}
// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound

void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		// prevents OCR3A from underflowing, using prescaler 64
		// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR3A = 0x0000; }
		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}
void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}
void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}
void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1=0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff() {
	TCCR1B = 0x00;
}
void TimerISR() {
	TimerFlag = 1;
}// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	//initialize and set inputs/outputs
	DDRB = 0xFF; PORTB = 0x00;
	
	PWM_on(); //turn on PWM
	TimerOn(); //turn timer on

	while(1)
	{	
		buttonStop =~PINA & 0x08;
		TimerSet(10); //set time
		TickPAC(); //call songs
		TickZelda();
		TickSW();
		
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}