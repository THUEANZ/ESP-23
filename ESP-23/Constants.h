//****************************
//  Constants
//****************************
// Constant values
const int ON_THRESH = 50;           // Set threshold value before NoteOn
const long MAX_PRESSURE = 3300000;  // Set maximum pressure that can blow
const long MIN_PRESSURE = 2900000;  // Set minimum pressureฃ

// Switch pins
const int NUM_SWITCHES = 18;  // Number of switches
const int switchPins[NUM_SWITCHES] = {
  22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39
};
const int DEBOUNCE_DELAY = 20;

// MIDI datas
const int MIDIchannel = 0;  // MIDI channel 1
const int startNote = 73;   // Start note is C#
const int C4_NOTE = 60; // C4 note
const int C5_NOTE = 72; // C5 note

// MIDI setting pins
const int PATCH_POT = A7;    // Patch select potentiometer pin
const int TUNE_POT = A6;     // Transpose select potentiometer pin
const int MODE_SWITCH = 53;  // Mode select pin

// VS1053 control pins
const int VS_XCS = 6;    // Control Chip Select pin
const int VS_XDCS = 7;   // Data Chip Select / BSYNC pin
const int VS_DREQ = 2;   // Data Request pin
const int VS_RESET = 8;  // Reset pin (active low)

// HX710B control pins
const int HX_DOUT = A4;  // Data Out pin
const int HX_CLK = A3;   // Clock pin

// Operating modes
const int PLAY_MODE = 0;   // Default mode
const int PATCH_MODE = 1;  // Patch change
const int TUNE_MODE = 2;   // Transpose

// Transpose ranges
int MIN_TUNE = -5;
int MAX_TUNE = 6;

// Key transpose letter to displays
char* KEY_TUNE[] = {
  "(G) ",  // -5
  "(Ab)",  // -4
  "(A) ",  // -3
  "(Bb)",  // -2
  "(B) ",  // -1
  "(C) ",  // 0
  "(Db)",  // +1
  "(D)",   // +2
  "(Eb)",  // +3
  "(E)",   // +4
  "(F)",   // +5
  "(Gb)"   // +6
};

// Plugin to put VS1053 into realtime MIDI mode
const unsigned short sVS1053b_Realtime_MIDI_Plugin[28] = { /* Compressed plugin */
  0x0007, 0x0001, 0x8050, 0x0006, 0x0014, 0x0030, 0x0715, 0xb080, /*    0 */
  0x3400, 0x0007, 0x9255, 0x3d00, 0x0024, 0x0030, 0x0295, 0x6890, /*    8 */
  0x3400, 0x0030, 0x0495, 0x3d00, 0x0024, 0x2908, 0x4d40, 0x0030, /*   10 */
  0x0200, 0x000a, 0x0001, 0x0050,
};