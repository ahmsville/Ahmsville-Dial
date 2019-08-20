#include "Ahmsville_dial.h"

/*


....By Ahmed Oyenuga (Ahmsville 2019).
*/

Ahmsville_dial::Ahmsville_dial() {

}

void Ahmsville_dial::initialize_ahmsvilleDial(int numoftouchinputs) {
	if (numoftouchinputs == 1) {
		Ahmsville_dialTouch.set_capTouchPins(15, 16, 0, 0, 0);   //sets the arduino pins used for the capacitive touch (sendpin, pad1, pad2, pad3, pad4)
	}
	else if (numoftouchinputs == 2) {
		Ahmsville_dialTouch.set_capTouchPins(15, 16, 0, 0, 0);   //sets the arduino pins used for the capacitive touch (sendpin, pad1, pad2, pad3, pad4)
	}
	else if (numoftouchinputs == 4) {
		Ahmsville_dialTouch.set_capTouchPins(15, 16, 0, 0, 0);   //sets the arduino pins used for the capacitive touch (sendpin, pad1, pad2, pad3, pad4)
	}
	Ahmsville_dialTouch.set_inputTypeThresholds(20, 40, 80, 120);  //sets the thresholds for the four input types  (singletap, shortpress, longpress, doubletapspeed)
	Ahmsville_dialTouch.set_adaptiveSensitivity(numoftouchinputs, 0.3, true);  //set touch sensitivity to auto
	Ahmsville_dialKnob.set_poleStateValues(256, 512, 768);   //set ADC values for the poles (northpole, neutral, southpole)
	set_haptics(9, 60, 20, 255);
	Ahmsville_dialTouch.initialize_capTouch(numoftouchinputs);
	Ahmsville_dialKnob.initialize_encoder();
	Keyboard.begin();
	Mouse.begin();
	delay(2000);
}

void Ahmsville_dial::initialize_ahmsvilleDialCustom(int touchpin) {

}

void Ahmsville_dial::set_touchDetectionThreshold(int padnum, double dth, double rth) {
	Ahmsville_dialTouch.set_detectionThreshold(padnum, dth, rth);
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

void Ahmsville_dial::normalize(int numoftouchinputs) {
	if (numoftouchinputs == 1) {
		Ahmsville_dialTouch.update_basevalueFromNoise(0);
	}
	else if (numoftouchinputs == 2) {
		Ahmsville_dialTouch.update_basevalueFromNoise(0);
		Ahmsville_dialTouch.update_basevalueFromNoise(1);
	}
	else if (numoftouchinputs == 4) {
		Ahmsville_dialTouch.update_basevalueFromNoise(0);
		Ahmsville_dialTouch.update_basevalueFromNoise(1);
		Ahmsville_dialTouch.update_basevalueFromNoise(2);
		Ahmsville_dialTouch.update_basevalueFromNoise(3);
	}
	Ahmsville_dialKnob.recaliberate_startPosition();
}

void Ahmsville_dial::normalizeCustom(int pad) {
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

void Ahmsville_dial::dialCommand_keyPress(char key) {
	Keyboard.press(key);
	delay(cdelay);
}

void Ahmsville_dial::dialCommand_movePointer(char axis, int amount) {
	if (axis == 'x') {
		Mouse.move((amount), 0, 0);
		delay(cdelay);
	}
	else if (axis == 'y') {
		Mouse.move(0, (amount), 0);
		delay(cdelay);
	}
}

void Ahmsville_dial::dialCommand_mouseClick(char button) {
	Mouse.click(button);
	delay(cdelay);
}

void Ahmsville_dial::dialCommand_mousePress(char button) {
	Mouse.press(button);
	delay(cdelay);
}

void Ahmsville_dial::dialCommand_Release(char button) {
	Keyboard.releaseAll();
	Mouse.release(button);
}