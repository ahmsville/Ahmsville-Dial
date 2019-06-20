#include "Ahmsville_dial.h"

/*


....By Ahmed Oyenuga (Ahmsville 2019).
*/

Ahmsville_dial::Ahmsville_dial() {

}

void Ahmsville_dial::initialize_ahmsvilleDial() {
	set_touchInputPins(15, 16, 0, 0, 0);
	set_touchTypeThresholds(20, 40, 80, 120);
	set_touchAutoSensitivity(0, 0.8, true);
	set_knobPoleStateValues(180, 512, 840);
	set_haptics(9, 25, 12, 200);
	initialize_ahmsvilleDialCustom(1);
	Keyboard.begin();
	Mouse.begin();
	delay(2000);
}

void Ahmsville_dial::initialize_ahmsvilleDialCustom(int numoftouchinputs) {
	Ahmsville_dialTouch.initialize_capTouch(numoftouchinputs);
	Ahmsville_dialKnob.initialize_encoder();
	Keyboard.begin();
	Mouse.begin();
}

void Ahmsville_dial::set_touchInputPins(int sp, int rp0, int rp1, int rp2, int rp3) {
	Ahmsville_dialTouch.set_capTouchPins(sp, rp0, rp1, rp2, rp3);
}

void Ahmsville_dial::set_touchTypeThresholds(int scl, int spr, int lpr, int dcls) {
	Ahmsville_dialTouch.set_inputTypeThresholds(scl, spr, lpr, dcls);
}

void Ahmsville_dial::set_touchDetectionThreshold(double dth, double rth) {
	Ahmsville_dialTouch.set_detectionThreshold(dth, rth);
}

void Ahmsville_dial::set_touchAutoSensitivity(int touchpin, double mastersensitivity, bool actv) {
	Ahmsville_dialTouch.set_adaptiveSensitivity(touchpin, mastersensitivity, actv);
}

void Ahmsville_dial::set_knobPoleStateValues(int np, int nu, int sp) {
	Ahmsville_dialKnob.set_poleStateValues(np, nu, sp);
}

void Ahmsville_dial::set_haptics(int pin, int durationtouch, int durationknob, int strength) {
	Ahmsville_dialTouch.set_haptics(pin, durationtouch, strength);
	Ahmsville_dialKnob.set_haptics(pin, durationknob, strength);
}

int Ahmsville_dial::capTouch(int pad) {
	int touch = Ahmsville_dialTouch.detect_touchFromNoise(pad);
	return touch;
}

int Ahmsville_dial::capTouch() {
	int touch = Ahmsville_dialTouch.detect_touchFromNoise(0);
	return touch;
}

int Ahmsville_dial::capTouch_single() {
	int touch = Ahmsville_dialTouch.detect_touchFromNoise_single(0);
	return touch;
}

int Ahmsville_dial::capTouch_single(int pad) {
	int touch = Ahmsville_dialTouch.detect_touchFromNoise_single(pad);
	return touch;
}

int Ahmsville_dial::knob() {
	int knob = Ahmsville_dialKnob.detect_rotation();
	return knob;
}

void Ahmsville_dial::normalize() {
	Ahmsville_dialTouch.update_basevalueFromNoise(0);
	Ahmsville_dialKnob.recaliberate_startPosition();
}

void Ahmsville_dial::normalize(int pad) {
	Ahmsville_dialKnob.recaliberate_startPosition();
	Ahmsville_dialTouch.update_basevalueFromNoise(pad);
}

int Ahmsville_dial::get_knobState(int sensornum) {
	int state = Ahmsville_dialKnob.get_currentSensorValue(sensornum);
	return state;
}

void Ahmsville_dial::dialCommand(char keystroke1, char keystroke2, char func) { //3 chain command shortcuts e.g CTRL+ALT+N
	Keyboard.press(keystroke1);
	delay(cdelay);
	Keyboard.press(keystroke2);
	delay(cdelay);
	Keyboard.press(func);
	delay(cdelay);
	Keyboard.releaseAll();
}

void Ahmsville_dial::dialCommand(char keystroke1, char func) { //2 chain command shortcuts e.g CTRL+N
	Keyboard.press(keystroke1);
	delay(cdelay);
	Keyboard.press(func);
	delay(cdelay);
	Keyboard.releaseAll();
}

void Ahmsville_dial::dialCommand(char func) { //single key shortcuts e.g Tab
	Keyboard.press(func);
	delay(cdelay);
	Keyboard.releaseAll();
}


