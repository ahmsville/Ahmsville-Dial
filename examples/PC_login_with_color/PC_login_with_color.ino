#include <Ahmsville_dial.h>
#include <FastLED.h>

/*
  Sample sketch of how to use the dial to login to you pc with a color.
  The sketch works by entering a predefined password sting into the password box if the Led color matches the specified keycolor.

  Turn knob to change color and single tap to select color.

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

int count = 0, touch, cdelay = 1;

long colors[9] = {CRGB::White, CRGB::Orange, CRGB::Purple, CRGB::Aqua, CRGB::Brown, CRGB::Red, CRGB::Yellow, CRGB::Lime, CRGB::Blue};  //color selection array
int numofcolors = 9; //specifies the number of colors in the colors array.
long keycolor = CRGB::Lime;  //select login color from the colors[] array
#define Password "password"  //define login password
long currentcolor;


void setup()
{
 ahmsville_dial.initialize_ahmsvilleDial(1);   //initialize ahmsville dial
  initializeLED();
}

void loop()
{
  count += ahmsville_dial.knob();  //detecting knob rotations (returns a signed integer in relation to the direction of the rotation)
  touch = ahmsville_dial.capTouch();  //detecting capacitive touch (returns integer 1 - 4 or 0 when no touch is detected)
  Login();
  ahmsville_dial.normalize(1);
}


void Login() {
  if (count < 0) {
    count = count + numofcolors;
    if (count < 0) {
      count = numofcolors - 1;
    }
  }

 fill_solid(leds, NUM_LEDS, colors[count]);  //update all leds
    FastLED.show();

  currentcolor = colors[count];
  if (ahmsville_dial.capTouch() == 1) {
    if (currentcolor == keycolor) {  //check if selected color is correct
      Keyboard.println(Password); //enter password
      delay(cdelay);
      Keyboard.press(KEY_RETURN);  //click enter
      Keyboard.releaseAll();
      selected();
    }
    else {  //if selected color is wrong
      error();
    }
  }
  if (count >= numofcolors) {
    count = 0;
  }
}

void initializeLED() {
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void selected() { //led animation to indicate a selection
  uint16_t dimtime = 60, dimrate = 5;
  for (int i = 0; i < dimtime; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      nblend(leds[j], CRGB::White, (dimtime / 3));
      FastLED.show();
    }
    delay(dimrate);
  }
  for (int i = 0; i < dimtime; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      nblend(leds[j], currentcolor, (dimtime / 3));
      FastLED.show();
    }
    delay(dimrate);
  }
}

void error() {  // blinks led twice
  int blinkspeed = 100;  //blink speed in ms
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(blinkspeed);
  fill_solid(leds, NUM_LEDS, currentcolor);
  FastLED.show();
  delay(blinkspeed);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(blinkspeed);
  fill_solid(leds, NUM_LEDS, currentcolor);
  FastLED.show();
}