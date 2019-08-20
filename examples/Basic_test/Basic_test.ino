#include <Ahmsville_dial.h>
#include <FastLED.h>

/*
    upload sketch to the dial to test all its functions.
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

void setup() {
 ahmsville_dial.initialize_ahmsvilleDial(1);   //initialize ahmsville dial
  initializeLED();
}

void loop() {
  rainbow();  // LED animation
  count += ahmsville_dial.knob();  //detecting knob rotations (returns a signed integer in relation to the direction of the rotation)
  touch = ahmsville_dial.capTouch();  //detecting capacitive touch (returns integer 1 - 4 or 0 when no touch is detected)
  Serial.print(count);  //print knob rotation count
  Serial.print("\t");
  if (touch == 1) {  //print "singletap" if touch type is 1
    Serial.println("singletap");
    delay(1000);
  }
  else if (touch == 2) { //print "doubletap" if touch type is 2
    Serial.println("doubletap");
    delay(1000);
  }
  else if (touch == 3) {  //print "shortpress" if touch type is 3
    Serial.println("shortpress");
    delay(1000);
  }
  else if (touch == 4) { //print "longpress" if touch type is 4
    Serial.println("longpress");
    delay(1000);
  }
  else if (touch == 0) { //print "no input" if touch type is 0
    Serial.println("No input");
  }
  ahmsville_dial.normalize(1);

}

/********************************************************************LED SETTINGS/ANIMATION*************************************************/

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
