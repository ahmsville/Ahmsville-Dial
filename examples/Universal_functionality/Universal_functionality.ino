#include <Ahmsville_dial.h>
#include <FastLED.h>

/*
 * This sketch combines all the functionalities of the other examples into one.

   ---(in app controls) Different applications are maped to different colors so you can switch between applications by switching colors.
   (longpress to enter selection mode, turn the knob to change application/color, single tap to activate, double tap to activate and quick launch the application).
   
   ---(Media control) Dial defaults to a media controller when in idle mode, the variable "sleeptime" determines how long it takes the dial to enter idle mode.
   (longpress to exit idle mode)
   
   ---(Pc login with color) When color is white, dial functions as a passKey to login to your pc with a predefined color.
   (turn the knob to find key color, single tap to enter password)
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
bool fadeswitch = 1;

int count = 0, c_count = 0, touch, transition = 1, hold = 350;
long timer = 0, sleeptime = 1; //sleeptime is how long it takes the dial to enter sleep/idle mode in minutes)
long colors[9] = {CRGB::White, CRGB::Orange, CRGB::Lime, CRGB::Purple, CRGB::Aqua, CRGB::Brown, CRGB::Red, CRGB::Yellow, CRGB::Blue};  //color selection array
int numofcolors = 9; //specifies the number of colors in the colors array.
long currentcolor = colors[0];
long keycolor = CRGB::Aqua;  //specifies password color.
#define Password     "password"
bool dualfunctiontouch[2] = {LOW, LOW};

/***************************************************setting keyboard shortcut keys**************************************************************/

char ctrl = KEY_LEFT_CTRL, alt = KEY_LEFT_ALT, shift = KEY_LEFT_SHIFT, left = KEY_LEFT_ARROW, right = KEY_RIGHT_ARROW;

//------------------------------media controller keys----------------------------------
char v_up = 'u';  // shift+alt+v_up will increase media volume
char v_down = 'y';  // shift+alt+v_down will decrease media volume
char play = 'p';  // shift+alt+play will play/pause media
char next = 'o';  // shift+alt+next will skip media forward
char prev = 'i';  // shift+alt+prev will skip media backward

char knobFunc[2] = {v_up, v_down};
int resetknob = 0, currentknobmode = 0, anim = 0;

//---------------------------application launch keys-------------------------------------
//assign specific letters to launch specific applications
char app1 = 'b';
char app2 = ' ';
char app3 = ' ';
char app4 = ' ';

//----------------------------application specific shortcut keys--------------------------
char Chrome[5] = {KEY_F5, 'k', 'f', KEY_PAGE_UP, KEY_PAGE_DOWN}; //maped to color orange
char Adobeuniversal[4] = {KEY_DOWN_ARROW, KEY_UP_ARROW, '-', '='}; //maped to color lime
char Premierepro[5] = {'i', 'o', 'f', KEY_LEFT_ARROW, KEY_RIGHT_ARROW}; //maped to color purple
char Altium[5] = {'p', 3, 'G', KEY_PAGE_UP, KEY_PAGE_DOWN}; //maped to color brown

void setup()
{
  ahmsville_dial.initialize_ahmsvilleDial();   //initialize ahmsville dial
  initializeLED();
}

void loop()
{
  ahmsville_dial.normalize();
  count += ahmsville_dial.knob();  //detecting knob rotations (returns a signed integer in relation to the direction of the rotation)
  touch = ahmsville_dial.capTouch();  //detecting capacitive touch (returns integer 1 - 4 or 0 when no touch is detected)
  update_LEDS();
  in_app_controls(currentcolor);
  if (touch == 4) {
    function_selector();//enter function selection by color
  }
  idle_time();  //keeps track of how long the dial is idle for.
}


/******************************************************************************************************************************************************************/
//this function keeps track of how long the dial is idle for
//this function puts the dial in sleep/idle mode if its not touched for the specified sleeptime.
//in sleep/idle mode, dial functions exclusively as media controller
//function will not run if dial is in continuous use
//longpress to exit sleep/idle mode

void idle_time() {
  long currenttime = millis();
  if (count == 0 && touch == 0) { //check if dial is untouched
    if ((currenttime - timer) >= (sleeptime*60000)) { //check if dial as been idle for more than the specified sleeptime
      while (touch != 4) { //enter sleep/idle mode (dial functions as a media controller) longpress to exit sleep/idle mode
        LEDanimation();  // LED animation
        Serial.println("idle");
        count += ahmsville_dial.knob();  //detecting knob rotations (returns a signed integer in relation to the direction of the rotation)
        touch = ahmsville_dial.capTouch();  //detecting capacitive touch (returns integer 1 - 4 or 0 when no touch is detected)
        Mediacontrol(); //media control function when in sleep/idle mode
        ahmsville_dial.normalize();
      }
      timer = millis();
      transition = 2;
    }
  } else {
    timer = millis();
  }
  count = 0;
  touch = 0;
}

/******************************************************************************************************************************************************************/
//This Function is used to change the operation of the dial for a specific application based on its assigned color
//Function also works for launching applications based on its specified color (achieved with a "doubletap" and a call of the function launchapp_bycolor()

void function_selector() {
  for (int i = 0; i < NUM_LEDS; i++) {  //update all leds
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(hold);
  }
  count = 0;
  touch = 0;
  while (touch != 1 && touch != 2) {
    count += ahmsville_dial.knob();  //detecting knob rotations (returns a signed integer in relation to the direction of the rotation)
    if (count < 0) {
      count = count + numofcolors;
      if (count < 0) {
        count = numofcolors - 1;
      }
    }
    else if (count >= numofcolors) {
      count = 0;
    }
    currentcolor = colors[count];
    update_LEDS();
    touch = ahmsville_dial.capTouch();
    if (touch == 1) {
      currentcolor = colors[count];
      selected();
    }
    else if (touch == 2) {
      currentcolor = colors[count];
      launchapp_bycolor(); //launch app based on selected color
      selected();
    }
    ahmsville_dial.normalize();
  }
}

/******************************************************************************************************************************************************************/
//This function performs in app controls based on the input from the dials capacitive touch and knob rotation.
//active application depends on selected dial color

void in_app_controls(long color) {
  if (color == colors[0]) {
    Login();
  }
  /***********************************************************************Orange (chrome)******************************************************************/
  else if (color == colors[1]) {
    //Touch Action
    if (touch == 1) { //app1 shortcut 1 maped to single tap on the dail
      ahmsville_dial.dialCommand(Chrome[0]);
    }
    else if (touch == 2) { //app1 shortcut 2 maped to double tap on the dail
      ahmsville_dial.dialCommand(ctrl, Chrome[1]);
    }
    else if (touch == 3) { //app1 shortcut 3 maped to shortpress tap on the dail
      ahmsville_dial.dialCommand(ctrl, Chrome[2]);
    }

    //Knob Action
    if (count > 0) { //app1 shortcut maped to clockwise rotation on the knob
      ahmsville_dial.dialCommand(ctrl, Chrome[4]);
    }
    else if (count < 0) { //app1 shortcut maped to anticlockwise rotation on the knob
      ahmsville_dial.dialCommand(ctrl, Chrome[3]);
    }
  }
/***************************************************************************Lime (adobeuniversal)******************************************************************/
  else if (color == colors[2]) {
    //Touch Action
    if (touch == 1) { //app2 shortcut 1 maped to single tap on the dail
      ahmsville_dial.dialCommand(Adobeuniversal[0]);
    }
    else if (touch == 2) { //app2 shortcut 2 maped to double tap on the dail
      if (dualfunctiontouch[0] == LOW) {
        dualfunctiontouch[0] = !dualfunctiontouch[0];
      } else {
        dualfunctiontouch[0] = !dualfunctiontouch[0];
      }
    }
  else if (touch == 3) { //app2 shortcut 3 maped to shortpress tap on the dail
    ahmsville_dial.dialCommand(Adobeuniversal[2]);
  }

  //Knob Action
  if (count > 0) { //app2 shortcut maped to clockwise rotation on the knob
    if (dualfunctiontouch[0] == LOW) {
      ahmsville_dial.dialCommand(Adobeuniversal[1]);
    } else {
      ahmsville_dial.dialCommand(ctrl, Adobeuniversal[3]);
    }

  }
  else if (count < 0) { //app2 shortcut maped to anticlockwise rotation on the knob
    if (dualfunctiontouch[0] == LOW) {
      ahmsville_dial.dialCommand(Adobeuniversal[0]);
    } else {
      ahmsville_dial.dialCommand(ctrl, Adobeuniversal[2]);
    }
  }
}
  
  /*********************************************************************Purple (Premier pro)*****************************************************************************/
  else if (color == colors[3]) {
    //Touch Action
    if (touch == 1) { //app2 shortcut 1 maped to single tap on the dail
      ahmsville_dial.dialCommand(Premierepro[0]);
    }
    else if (touch == 2) { //app2 shortcut 2 maped to double tap on the dail
      ahmsville_dial.dialCommand(Premierepro[1]);
    }
    else if (touch == 3) { //app2 shortcut 3 maped to shortpress tap on the dail
      ahmsville_dial.dialCommand(ctrl, shift, Premierepro[2]);
    }

    //Knob Action
    if (count > 0) { //app2 shortcut maped to clockwise rotation on the knob
      ahmsville_dial.dialCommand(Premierepro[4]);
    }
    else if (count < 0) { //app2 shortcut maped to anticlockwise rotation on the knob
      ahmsville_dial.dialCommand(Premierepro[3]);
    }
  }
  /*********************************************************************Brown (Altium)***********************************************************************/
  else if (color == colors[5]) {
    //Touch Action
    if (touch == 1) { //app2 shortcut 1 maped to single tap on the dail
      ahmsville_dial.dialCommand(Altium[0]);
    }
    else if (touch == 2) { //app2 shortcut 2 maped to double tap on the dail
      if (dualfunctiontouch[1] == LOW) {
        ahmsville_dial.dialCommand(3);
        dualfunctiontouch[1] = !dualfunctiontouch[1];
      } else {
        ahmsville_dial.dialCommand(2);
        dualfunctiontouch[1] = !dualfunctiontouch[1];
      }
    }
    else if (touch == 3) { //app2 shortcut 3 maped to shortpress tap on the dail
      ahmsville_dial.dialCommand(Altium[2]);
    }

    //Knob Action
    if (count > 0) { //app2 shortcut maped to clockwise rotation on the knob
      ahmsville_dial.dialCommand(Altium[4]);
    }
    else if (count < 0) { //app2 shortcut maped to anticlockwise rotation on the knob
      ahmsville_dial.dialCommand(Altium[3]);
    }
  }
  
}

//This function is used to launch specific applications based on is color

void launchapp_bycolor() {
  if (currentcolor == colors[1]) { //color 1 for launching application 1 (note: colors starts from element 2 in the array colors[])
    ahmsville_dial.dialCommand(ctrl, alt, app1); //lauch application with 3 chain command CTRL+ALT+app1
  }
  else if (currentcolor == colors[2]) { //color 2 for launching application 2 (note: colors starts from element 2 in the array colors[])
    ahmsville_dial.dialCommand(ctrl, alt, app2); //lauch application with 3 chain command CTRL+ALT+app1
  }
}

//This function controls PC media based on the input from the dials capacitive touch and knob rotation; with the dialCommand() functions
//dialCommand() can be called with three chain shortcuts (CTRL+ALT+function), two Chain shortcuts (CTRL+function) and single key shortcut (PAGE UP)

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
  else {
    ///
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

/******************************************************************************************************************************************************************/
//This function is used to login to a PC by printing the password followed by a the Return key.
//PC password should be specified and dial color must match the specified keycolor
//turn knob to select color and single tap to select the color/enter password

void Login() {
  if (count != 0) {
    while (touch != 1) {
      c_count += ahmsville_dial.knob();  //detecting knob rotations (returns a signed integer in relation to the direction of the rotation)
      if (c_count < 0) {
        c_count = c_count + numofcolors;
        if (c_count < 0) {
          c_count = numofcolors - 1;
        }
      }

      for (int i = 0; i < NUM_LEDS; i++) {  //update all leds
        leds[i] = colors[c_count];
        FastLED.show();
      }
      currentcolor = colors[c_count];
      touch = ahmsville_dial.capTouch_single();
      if (touch == 1) {
        if (currentcolor == keycolor) {  //check if selected color is correct
          Keyboard.println(Password); //enter password
          delay(5);
          Keyboard.press(KEY_RETURN);  //click enter
          Keyboard.releaseAll();
          selected();
          delay(hold);
        }
        else {  //if selected color is wrong
          error();
          delay(hold);
        }
        currentcolor = colors[0];
      }
      if (c_count >= numofcolors) {
        c_count = 0;
      }
      ahmsville_dial.normalize();
    }
  }
  count = 0;
}

/********************************************************************LED SETTINGS/ANIMATION*************************************************/

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
  transition = 2;
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
  transition = 2;
}

void update_LEDS() {
  if (transition == 2) {
    uint16_t dimtime = 60, dimrate = 5;
    for (int i = 0; i < dimtime; i++) {
      for (int j = 0; j < NUM_LEDS; j++) {
        nblend(leds[j], currentcolor, (dimtime / 3));
        FastLED.show();
      }
      delay(dimrate);
    }
    transition = 1;
  }
  else if (transition == 1) {
    fill_solid(leds, NUM_LEDS, currentcolor);
    FastLED.show();
  }
}

void LEDanimation() {
  if (anim == 0) {
    rainbow();
  }
  else if (anim == 1) {
    sinelon();
  }
  else if (anim == 2) {
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

void initializeLED() {
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}