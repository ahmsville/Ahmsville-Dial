#ifndef Ahmsville_dial_h
#define Ahmsville_dial_h

#include "Arduino.h"
#include <AdvCapTouch.h>
#include <MagRotaryEncoding.h>
#include "Keyboard.h"
#include "Mouse.h"

class Ahmsville_dial {
private:
	MagRotaryEncoder Ahmsville_dialKnob = MagRotaryEncoder(A1, A0); // create new encoding instance and specify the arduino pins connected to the hall effect sensors
	AdvCapTouch Ahmsville_dialTouch = AdvCapTouch();  //create a new captouch instance
	int cdelay = 1;

public:
	Ahmsville_dial();
	void set_touchDetectionThreshold(int padnum, double dth, double rth);   //set capacitive touch sensitivity in the form of detection, rejection thresholds values
	void initialize_ahmsvilleDial(int numoftouchinputs);  //initialize ahmsville dial
	void initialize_ahmsvilleDialCustom(int touchpin);  //initialize ahmsville dial with custom settings
	void set_haptics(int pin, int durationtouch, int durationknob, int strength);  //set arduino haptics variables (arduino pwm pin connected to viberation motor, duration of haptics(ms), strength from 0-255)
	
	/*---------------------------------------------------------Dial Inputs functions (capacitive touch & Knob rotation)-----------------------------------------------------*/

	int capTouch(int pad);   //detect capacitive touch from specified pad
	int capTouch();   //detect capacitive touch
	int capTouch_single(int pad); //detect single tap alone from specified pad
	int capTouch_single();  //detect single tap alone
	int knob();    //detect rotation

	/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

	void normalize(int numoftouchinputs);
	void normalizeCustom(int pad);
	int get_knobState(int sensornum);

	/*-------------------------------------------------------Dial keyboard shortcut functions-------------------------------------------------------------------------------*/

	void dialCommand(char keystroke1, char keystroke2, char func); //3 chain command shortcuts e.g CTRL+ALT+N
	void dialCommand(char keystroke1, char func);  //2 chain command shortcuts e.g CTRL+N
	void dialCommand(char func); //single key shortcuts e.g Tab
	void dialCommand_keyPress(char key);
	void dialCommand_movePointer(char axis, int amount);
	void dialCommand_mouseClick(char button);
	void dialCommand_mousePress(char button);
	void dialCommand_Release(char button);

};








#endif // !Ahmsville_dial_h

