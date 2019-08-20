#include <Ahmsville_dial.h>
#include <FastLED.h>

/*
 This sketch shows how to use the dial to manipulate 3D models in CAD softwares like fusion 360 and solidworks.

 By default the sketch works with solidworks. 
 Fusion 360 users might need to change their pan, rotate and zoom setting under preference to solidworks.

 --double tap the dial to toggle between spinning and zooming the model.
 --single tap to change model view to isometric (solidworks only).
 
    
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
int count = 0, touch, anim = 0;
bool currentknobmode = 0;
bool fadeswitch = 1;
int command_delay = 2;

char ctrl = KEY_LEFT_CTRL, shift = KEY_LEFT_SHIFT;

void setup() {
  ahmsville_dial.initialize_ahmsvilleDial(1);   //initialize ahmsville dial
  initializeLED();
}

void loop() {
  LEDanimation();  // LED animation
  count = ahmsville_dial.knob();  //detecting knob rotations (returns a signed integer in relation to the direction of the rotation)
  touch = ahmsville_dial.capTouch();  //detecting capacitive touch (returns integer 1 - 4 or 0 when no touch is detected)
  CAD_Control();  //cad control function
  ahmsville_dial.normalize(1);
}

void CAD_Control(){
  if(touch == 2){  //toggle knob operation (spin or zoom model)
    currentknobmode = !currentknobmode;
    if(currentknobmode == 0){
      anim = 0;
    }else if(currentknobmode == 1){
      anim = 1;
    }
  }else if (touch == 1){  //set model view to isometric (solidworks)
    Keyboard.press(ctrl);
    delay(command_delay);
     Keyboard.press('7');
     delay(command_delay);
     Keyboard.releaseAll();
  }
 if(currentknobmode == 0){  //spinning operation
  if (count != 0) {
    Mouse.press(MOUSE_MIDDLE);
    delay(command_delay);
         Mouse.move(-(count*7), 0, 0); 
         delay(command_delay);
         Mouse.release(MOUSE_MIDDLE);
  }
 }else if (currentknobmode == 1){  //zooming operation
  if (count != 0) {
  Keyboard.press(shift);
  delay(command_delay);
   Mouse.press(MOUSE_MIDDLE);
   delay(command_delay);
    Mouse.move(0, -(count*7), 0); 
    delay(command_delay);
    Keyboard.releaseAll();
     Mouse.release(MOUSE_MIDDLE);
  }
 }
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
