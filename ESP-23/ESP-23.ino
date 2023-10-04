//**********************************************************
//  Includes
//**********************************************************
#include <Wire.h>     // I2C communication
#include <SPI.h>      // SPI communication
#include <HX710.h>    // Pressure sensor
#include <U8g2lib.h>  // OLED control
#include <BLEMidi.h>  // MIDI through BLE

#include "Variables.h"
#include "Constants.h"
#include "Bitmaps.h"

//**********************************************************
//  Main program loop
//**********************************************************
void loop() {

  // ***********************
  // Display update routine
  // ***********************
  currentDisplayTime = millis();
  if ((currentDisplayTime - previousDisplayTime) >= 100) {
    previousDisplayTime = currentDisplayTime;

    // Update the display if necessary
    if (displayChanged == true) {  // If display need to update
      changeDisplay();             // Display something
      displayChanged = false;      // Clear flag
    }
  }

  // *********************
  // Main program routine
  // *********************
  currentProgramTime = millis();
  if ((currentProgramTime - previousProgramTime) >= 10) {
    previousProgramTime = currentProgramTime;

    // *********************
    // Read breath pressure
    // *********************
    lastRawPressure = rawPressure;  // Save last raw pressure value for average

    while (!airPress.isReady())  // Wait for sensor to be ready
      ;
    airPress.readAndSelectNextData(HX710_DIFFERENTIAL_INPUT_40HZ);                       // Select sensor data rate
    rawPressure = airPress.getLastDifferentialInput();                                   // Get sensor value
    breath = (lastRawPressure + rawPressure) / 2;                                        // Average sensor value
    breath = map(breath, MIN_PRESSURE + blowResist, MAX_PRESSURE + blowResist, 0, 127);  // Convert into MIDI data
    breath = constrain(breath, 0, 127);                                                  // Constrain value in MIDI range

    if (breath <= ON_THRESH) {  // Breath not enough to trigger sound
      breath = 0;               // Cut off pressure to zero
    }

    // ********************************
    // Get MIDI note from Arduino NANO
    // ********************************
    if (Serial.available() > 0) {
      midiNote = Serial.read();  // Read MIDI note through serial
    }

    pitch = midiNote + tuning;  // Transpose note from instrument's key

    // **************
    // Start blowing
    // **************
    if ((breath > ON_THRESH) && (lastPitch == 0)) {  // Have enough pressure to trigger sound and no note play before
      // BLE mode
      if (bleConnected == true) {
        BLEMidiServer.noteOn(MIDIchannel, pitch, breath);
      }
      // Normal mode
      else {
        noteOn(MIDIchannel, pitch, breath);  // Play note
      }

      lastPitch = pitch;    // Keep current playing note
      lastBreath = breath;  // Keep current playing pressure
    }

    // ********************************
    // Note change while still blowing
    // ********************************
    else if ((breath > ON_THRESH) && (lastPitch > 0) && (pitch != lastPitch)) {  // Still playing but note changed
      // BLE mode
      if (bleConnected == true) {
        BLEMidiServer.noteOff(MIDIchannel, lastPitch, 0);  // Turn off previous note sound
        BLEMidiServer.noteOn(MIDIchannel, pitch, breath);  // Play new note
      }
      // Normal mode
      else {
        noteOff(MIDIchannel, lastPitch, 0);  // Turn off previous note sound
        noteOn(MIDIchannel, pitch, breath);  // Play new note
      }

      lastPitch = pitch;  // Keep new playing note
    }

    // *************
    // Stop blowing
    // *************
    else if ((breath <= ON_THRESH) && (lastPitch > 0)) {  // Pressure not enough to trigger sound
      // BLE mode
      if (bleConnected == true) {
        BLEMidiServer.noteOff(MIDIchannel, lastPitch, 0);  // Turn off current note
      }
      // Normal mode
      else {
        noteOff(MIDIchannel, lastPitch, 0);  // Turn off current note
      }

      lastPitch = 0;  // No note playing
    }

    // **********************************
    // Still blowing but pressure change
    // **********************************
    if ((breath > ON_THRESH) && (lastBreath != breath)) {  // Still playing but pressure changed
      // BLE mode
      if (bleConnected == true) {
        BLEMidiServer.controlChange(MIDIchannel, 2, breath);  // Breath controller control change (CC#2)
      }
      // Normal mode
      else {
        talkMIDI(0xB0, 11, breath);  // Expression control change (CC#11)
      }

      lastBreath = breath;  // Keep new playing pressure
    }

    // *******************
    // Handle Mode change
    // *******************
    if (mode == MENU_MODE) {
      // Scroll up
      if (digitalRead(UP_BUTTON) == LOW) {
        if (buttonUpState == 1) {
          buttonUpState = 0;
          selectedMenu = selectedMenu - 1;
          if (selectedMenu < 0) {
            selectedMenu = NUM_MENU - 1;
          }
          displayChanged = true;
        }
      } else {
        buttonUpState = 1;
      }
      // Scroll down
      if (digitalRead(DOWN_BUTTON) == LOW) {
        if (buttonDownState == 1) {
          buttonDownState = 0;
          selectedMenu = selectedMenu + 1;
          if (selectedMenu > NUM_MENU - 1) {
            selectedMenu = 0;
          }
          displayChanged = true;
        }
      } else {
        buttonDownState = 1;
      }

      // Set correct values for the previous and next menus
      previousMenu = selectedMenu - 1;
      if (previousMenu < 0) {
        previousMenu = NUM_MENU - 1;
      }
      nextMenu = selectedMenu + 1;
      if (nextMenu > NUM_MENU - 1) {
        nextMenu = 0;
      }

      // Select menu
      if (digitalRead(SELECT_BUTTON) == LOW) {
        if (buttonSelectState == 1) {
          buttonSelectState = 0;
          mode = selectedMenu;
          displayChanged = true;
        }
      } else {
        buttonSelectState = 1;
      }
      // Not in menu screen
    } else {
      // Select menu
      if (digitalRead(SELECT_BUTTON) == LOW) {
        if (buttonSelectState == 1) {
          buttonSelectState = 0;
          mode = MENU_MODE;
          displayChanged = true;
        }
    } else {
        buttonSelectState = 1;
      }
    }
    // *******************************
    // Handle mode function
    // *******************************
    switch (mode) {
      case PATCH_MODE:
        patchChange();
        break;
      case TUNE_MODE:
        transpose();
        break;
      case RESIST_MODE:
        adjustResist();
        break;
      case PRACTICE_MODE:
        displayChanged = true;
        break;
      case BLUETOOTH_MODE:
        bleStatus();
        blePair();
        digitalWrite(MUTE_INPUT, HIGH);  // Mute speaker
        break;
    }
    // If not in bluetooth mode
    if (mode != BLUETOOTH_MODE && isBleOn == true) {
      BLEMidiServer.end();  // End ble server
      isBleOn = false;
      digitalWrite(MUTE_INPUT, LOW);  // Unmute speaker
    }
  }
}
