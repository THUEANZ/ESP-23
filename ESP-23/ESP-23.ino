//****************************
//  Includes
//****************************
#include <Wire.h>
#include <SPI.h>
#include <HX710.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include "Variables.h"
#include "Constants.h"

//****************************
//  Main program loop
//****************************
void loop() {

  //*********************
  //*** 10 Hz routine ***
  //*********************
  current10HzTime = millis();
  if ((current10HzTime - previous10HzTime) >= 100) {
    previous10HzTime = current10HzTime;
    //update the display if necessary
    if (displayChanged == true) {  //if something changed
      changeDisplay();             //output to the display
      displayChanged = false;      //clear the flag
    }
  }

  //*********************
  //*** 100 Hz routine **
  //*********************
  current100HzTime = millis();
  if ((current100HzTime - previous100HzTime) >= 10) {
    previous100HzTime = current100HzTime;

    readPressure();

    readSwitches();

    pitch = tuning + startNote + 5 * LHf - 2 * LH1 - (LHb && !(LH1 && LH2)) - LH2 - (LH2 && LH1) - 2 * LH3 + LHp1 - LHp2 - 2 * LHp3 - RH1 - (RH1 && LH3) - RH2 - 2 * RH3 + RHp1 - 2 * RHp2 + RHs + 12 * Oct;

    // Serial.println(screen);

    // Start blowing
    if ((breath > ON_THRESH) && (lastPitch == 0)) {
      noteOn(MIDIchannel, pitch, breath);
      lastPitch = pitch;
      lastBreath = breath;
    }

    // Note change while still blowing
    else if ((breath > ON_THRESH) && (lastPitch > 0) && (pitch != lastPitch)) {
      noteOff(MIDIchannel, lastPitch, 0);
      noteOn(MIDIchannel, pitch, breath);
      lastPitch = pitch;
    }

    // Stop blowing
    else if ((breath <= ON_THRESH) && (lastPitch > 0)) {
      noteOff(MIDIchannel, lastPitch, 0);
      lastPitch = 0;
    }

    // Still blowing but pressure change
    if ((breath > ON_THRESH) && (breath != lastBreath)) {
      talkMIDI(0xB0, 0x0B, breath);  // Expression (CC#11)
      lastBreath = breath;
    }

    // Handle Mode switch
    if (modeSW == 1) {
      if (modeSwitchState == 1) {
        modeSwitchState = 0;
        mode += 1;
        if (mode > TUNE_MODE) {
          mode = PLAY_MODE;
        }
      }
    } else {
      modeSwitchState = 1;
    }

    // Handle each mode
    switch (mode) {
      case PLAY_MODE:
        screen = 0;
        displayChanged = true;        // Update display
        break;
      case PATCH_MODE:
        screen = 1;
        patchChange();
        break;
      case TUNE_MODE:
        screen = 2;
        transpose();
        break;
    }
  }
}