#include <Ahmsville_dial.h>
#include <FastLED.h>

/*
  Sample sketch for using the Dial to launch applications
  
   ---------------------Windows---------------------
    Right click on the shortcut icon for the application you wish to launch with the dial's touch inputs
    open properties and add desired keyboard shortcut under "shortcut key"

    this example uses single tap and double tap to launch two designated applications.---(CTRL + ALT + B) & (CTRL + ALT + C) respectively

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
int touch;

/***************************************************setting keyboard shortcut keys**************************************************************/

char ctrl = KEY_LEFT_CTRL, alt = KEY_LEFT_ALT, shift = KEY_LEFT_SHIFT, left = KEY_LEFT_ARROW, right = KEY_RIGHT_ARROW;
char appshortcuts[4] = {
  'b',  //assign specific letters to launch specific applications
  'c',
  ' ',   //shortpress and longpress not used
  ' '
};

void setup()
{
  ahmsville_dial.initialize_ahmsvilleDial(1);   //initialize ahmsville dial
  initializeLED();
}

void loop()
{
  rainbow();  // LED animation
  touch = ahmsville_dial.capTouch();  //detecting capacitive touch (returns integer 1 - 4 or 0 when no touch is detected)
  if (touch != 0){
  ahmsville_dial.dialCommand(ctrl, alt, appshortcuts[touch - 1]); //function to launch application based on detected touch input
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
