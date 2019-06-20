#include <Ahmsville_dial.h>
#include <FastLED.h>

/*
   Sample sketch for using the Dial as a media controller for your PC
   
   ---------------------Windows---------------------
    Download and install autohotkey from - https://autohotkey.com/download/
    Download Media_control script from - https://drive.google.com/open?id=19c7-OofvXPggWWJ0wtWSpAH8Yd8I73yD
    Right click on the script to run it.
    upload sketch to the dial and it should be able to control you PC Media.

    Follow the instructions on - https://www.maketecheasier.com/schedule-autohotkey-startup-windows/  to make the script run at startup.

    The Knob controls volume by default, shortpress and the knob functions as track selector, double tap and the knob controls fastforward and rewind.

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
bool fadeswitch = 1;

/***************************************************setting keyboard shortcut keys**************************************************************/

char ctrl = KEY_LEFT_CTRL, alt = KEY_LEFT_ALT, shift = KEY_LEFT_SHIFT, left = KEY_LEFT_ARROW, right = KEY_RIGHT_ARROW;
char v_up = 'u';  // shift+alt+v_up will increase media volume
char v_down = 'y';  // shift+alt+v_down will decrease media volume
char play = 'p';  // shift+alt+play will play/pause media
char next = 'o';  // shift+alt+next will skip media forward
char prev = 'i';  // shift+alt+prev will skip media backward
char mute = 'm';  //  shift+alt+mute will mute media

char knobFunc[2] = {v_up, v_down};
int resetknob = 0, currentknobmode = 0, anim = 0;

void setup()
{
  ahmsville_dial.initialize_ahmsvilleDial();   //initialize ahmsville dial
  initializeLED();
}

void loop()
{
  LEDanimation();  // LED animation
  count += ahmsville_dial.knob();  //detecting knob rotations (returns a signed integer in relation to the direction of the rotation)
  touch = ahmsville_dial.capTouch();  //detecting capacitive touch (returns integer 1 - 4 or 0 when no touch is detected)
  Mediacontrol(); // media control function
  ahmsville_dial.normalize();
}

void Mediacontrol () {
  /******************************************************************************TOUCH CONTROLS************************************************************/

  if (touch == 1) { //single tap to pause/play
    ahmsville_dial.dialCommand(shift, alt, play);
  }
  else if (touch == 2) { //double tap to seek media with the knob
    knobFunctionChanger(ctrl, ctrl, 1);
  }
  else if (touch == 3) { //shortpress to change media with the knob
    knobFunctionChanger(next, prev, 2);
  }
  else if (touch == 4) { //longpress to mute
    ahmsville_dial.dialCommand(shift, alt, mute);
  }

  /************************************************************************KNOB CONTROLS*****************************************************************/

  if (count > 0) { // knob clockwise
    if (currentknobmode == 1) {
      ahmsville_dial.dialCommand(shift, right);
    }
    else {
      ahmsville_dial.dialCommand(shift, alt, knobFunc[0]);
    }
  }
  else if (count < 0) { // knob anticlockwise
    if (currentknobmode == 1) {
      ahmsville_dial.dialCommand(shift, left);
    }
    else {
      ahmsville_dial.dialCommand(shift, alt, knobFunc[1]);
    }
  }

  /************************************************************************RESET KNOB CONTROLS TO DEFAULT************************************************************/

  if (currentknobmode != 0) {
    if (count == 0) {
      resetknob++;
    } else {
      resetknob = 0;
    }
  } else {
    ///
  }
  if (resetknob == 300) {  //reset knob as volume control after 300 counts
    knobFunctionChanger(v_up, v_down, 0);
    resetknob = 0;
  }
  count = 0;
}

void knobFunctionChanger(char cw, char ccw, int mode) {
  knobFunc[0] = cw;
  knobFunc[1] = ccw;
  currentknobmode = mode;
  anim = mode;
}

/********************************************************************LED SETTINGS/ANIMATION*************************************************/

void initializeLED() {
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void LEDanimation(){
  if (anim == 0){
    rainbow();
  }
   else if (anim == 1){
    sinelon();
  }
   else if (anim == 2){
    switchfade();
  }
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

void sinelon()
{
  EVERY_N_MILLISECONDS( 20 ) {   //frames/milliseconds delay
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy( leds, NUM_LEDS, 20);
    int pos = beatsin16( 30, 0, NUM_LEDS - 1 );
    leds[pos] += CHSV( gHue, 255, 192);
    FastLED.show();
  }
  EVERY_N_MILLISECONDS( 30 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
}

void switchfade() {
  EVERY_N_MILLISECONDS( 20 ) {    //frames/milliseconds delay
    EVERY_N_MILLISECONDS( 600 ) {
      fadeswitch = !fadeswitch;
    }
    int faderate = 30;
    if (fadeswitch == 1) {
      for (int i = 0; i < NUM_LEDS / 2; i++) {
        nblend(leds[i], CHSV( gHue, 255, 192), faderate);
        nblend(leds[NUM_LEDS - (i + 1)], CRGB::Black, faderate);
      }
    }
    else {
      for (int i = NUM_LEDS - 1; i >= NUM_LEDS / 2; i--) {
        nblend(leds[i], CHSV( gHue, 255, 192), faderate);
        nblend(leds[(NUM_LEDS - 1) - i], CRGB::Black, faderate);
      }
    }
    FastLED.show();
    EVERY_N_MILLISECONDS( 30 ) {
      gHue++;  // slowly cycle the "base color" through the rainbow
    }
  }
}