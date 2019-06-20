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
	void set_touchInputPins(int sp, int rp0, int rp1, int rp2, int rp3);  //sets the arduino pins used for the capacitive touch (sendpin, pad1, pad2, pad3, pad4)
	void set_touchTypeThresholds(int scl, int spr, int lpr, int dcls);  //sets the thresholds for the four input types  (singletap, shortpress, longpress, doubletapspeed)
	void set_touchDetectionThreshold(double dth, double rth);   //set capacitive touch sensitivity in the form of detection, rejection thresholds values
	void set_touchAutoSensitivity(int touchpin, double mastersensitivity, bool actv);  //set touch sensitivity to auto
	void set_knobPoleStateValues(int np, int nu, int sp);   //set ADC values for the poles (northpole, neutral, southpole)
	void initialize_ahmsvilleDial();  //initialize ahmsville dial
	void initialize_ahmsvilleDialCustom(int touchpin);  //initialize ahmsville dial with custom settings
	void set_haptics(int pin, int durationtouch, int durationknob, int strength);  //set arduino haptics variables (arduino pwm pin connected to viberation motor, duration of haptics(ms), strength from 0-255)
	
	/*---------------------------------------------------------Dial Inputs functions (capacitive touch & Knob rotation)-----------------------------------------------------*/

	int capTouch(int pad);   //detect capacitive touch from specified pad
	int capTouch();   //detect capacitive touch
	int capTouch_single(int pad); //detect single tap alone from specified pad
	int capTouch_single();  //detect single tap alone
	int knob();    //detect rotation

	/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

	void normalize();
	void normalize(int pad);
	int get_knobState(int sensornum);

	/*-------------------------------------------------------Dial keyboard shortcut functions-------------------------------------------------------------------------------*/

	void dialCommand(char keystroke1, char keystroke2, char func); //3 chain command shortcuts e.g CTRL+ALT+N
	void dialCommand(char keystroke1, char func);  //2 chain command shortcuts e.g CTRL+N
	void dialCommand(char func); //single key shortcuts e.g Tab

};








#endif // !Ahmsville_dial_h

