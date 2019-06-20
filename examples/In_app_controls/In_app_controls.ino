#include <Ahmsville_dial.h>
#include <FastLED.h>

/*
  Sample sketch for controlling designated actions with the dial in a specific application
  This example is configured to work with google chrome.

  ---single tap dial to refresh page
  ---double tap for quick google search
  ---shortpress to search page
  ---longpress to reopen recently closed tabs
  ---turn knob to switch between tabs

  Application specific shortcut keys is set with the appfinctions[] array.

  ....By Ahmsville...
*/


Ahmsville_dial ahmsville_dial = Ahmsville_dial(); //create a new ahmsville dial instance

/********************************************************************LED CONFIGURATION*****************************************************/
#define DATA_PIN    10
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    4
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         127

uint8_t gHue = 0; // rotating "base color"
int count = 0, touch;

/***************************************************setting keyboard shortcut keys**************************************************************/

char ctrl = KEY_LEFT_CTRL, alt = KEY_LEFT_ALT, shift = KEY_LEFT_SHIFT, left = KEY_LEFT_ARROW, right = KEY_RIGHT_ARROW;
char appfunctions[6] = {
  KEY_F5,  //functions 1 - 4 are maped to the touch inputs of the dail (1 = singletap, 2 = doubletap, 3 = shortpress and 4 = longpress)
  'k',
  'f',
  't',
  KEY_PAGE_UP,  //functions 5 & 6 are maped to the rotation of the knob, ideal for similar but upposing functions e.g zoom IN and zoom OUT.
  KEY_PAGE_DOWN
};

void setup()
{
  ahmsville_dial.initialize_ahmsvilleDial();   //initialize ahmsville dial
  initializeLED();
}

void loop()
{
  rainbow();  // LED animation
  count = ahmsville_dial.knob();  //detecting knob rotations (returns a signed integer in relation to the direction of the rotation)
  touch = ahmsville_dial.capTouch();  //detecting capacitive touch (returns integer 1 - 4 or 0 when no touch is detected)
  if (touch == 1) { //singletap
    In_app_touch_controls(appfunctions[0]); // application touch control function
  }
  else if (touch == 2) {  //doubletap
    In_app_touch_controls(ctrl, appfunctions[1]); // application touch control function
  }
  else if (touch == 3) {  //shortpress
    In_app_touch_controls(ctrl, appfunctions[2]); // application touch control function
  }
  else if (touch == 4) {   //longpress
    In_app_touch_controls(ctrl, shift, appfunctions[3]); // application touch control function
  }
  else if (touch == 0) {  //no input
    //do nothing
  }
  In_app_knob_controls(ctrl, count); // application knob control function
  ahmsville_dial.normalize();
}

void In_app_touch_controls(char keystroke1, char keystroke2, char func) { //touch function for 3 chain command shortcuts e.g CTRL+ALT+N
  //Touch Action
  ahmsville_dial.dialCommand(keystroke1, keystroke2, func);
}

void In_app_touch_controls(char keystroke1, char func) { //touch function for 2 chain command shortcuts e.g CTRL+N
  //Touch Action
  ahmsville_dial.dialCommand(keystroke1, func);
}

void In_app_touch_controls(char func) { //touch function for single key shortcuts e.g PAGE UP
  //Touch Action
  ahmsville_dial.dialCommand(func);
}

void In_app_knob_controls(char keystroke1, char keystroke2, int count) { //knob function for 3 chain command shortcuts e.g CTRL+ALT+N
  //Knob Action
  if (count < 0) { //anticlockwise knob rotation
    ahmsville_dial.dialCommand(keystroke1, keystroke2, appfunctions[4]);
  }
  else if (count > 0) { //clockwise knob rotation
    ahmsville_dial.dialCommand(keystroke1, keystroke2, appfunctions[5]);
  }
}

void In_app_knob_controls(char keystroke1, int count) { //knob function for 2 chain command shortcuts e.g CTRL+N
  //Knob Action
  if (count < 0) { //anticlockwise knob rotation
    ahmsville_dial.dialCommand(keystroke1, appfunctions[4]);
  }
  else if (count > 0) { //clockwise knob rotation
    ahmsville_dial.dialCommand(keystroke1, appfunctions[5]);
  }
}

void In_app_knob_controls(int count) { //touch function for single key shortcuts e.g PAGE UP
  //Knob Action
  if (count < 0) { //anticlockwise knob rotation
    ahmsville_dial.dialCommand(appfunctions[4]);
  }
  else if (count > 0) { //clockwise knob rotation
    ahmsville_dial.dialCommand(appfunctions[5]);
  }
}

void initializeLED() {
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void rainbow() {
  EVERY_N_MILLISECONDS( 20 ) {  //frames/milliseconds delay
    // FastLED's built-in rainbow generator
    fill_rainbow( leds, NUM_LEDS, gHue, 10);
    // send the 'leds' array out to the actual LED strip
    FastLED.show();
  }
  EVERY_N_MILLISECONDS( 30 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
}