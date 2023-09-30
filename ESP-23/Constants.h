//**********************************************************
//  Constants
//**********************************************************
// Constant values
const int ON_THRESH = 60;           // Set threshold value before NoteOn
const long MAX_PRESSURE = 3400000;  // Set maximum pressure that can blow
const long MIN_PRESSURE = 2900000;  // Set minimum pressure
const long LEVEL_STEP = 50000;      // Set min/max pressure step value per level

// // Switch pins
// const int NUM_SWITCHES = 18;  // Number of switches
// const int switchPins[NUM_SWITCHES] = {
//   22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39
// };
const int DEBOUNCE_DELAY = 20;

// MIDI datas
const int MIDIchannel = 0;  // MIDI channel 1
// const int startNote = 73;   // Start note is C#
const int C4_NOTE = 60;  // C4 note
const int C5_NOTE = 72;  // C5 note

// MIDI setting pins
const int CONTROL_POT = 25;    // Transpose select potentiometer pin
const int SELECT_BUTTON = 26;  // Select button pin
const int UP_BUTTON = 27;      // Up button pin
const int DOWN_BUTTON = 13;    // Down button pin

// VS1053 control pins
const int VS_XCS = 5;     // Control Chip Select pin
const int VS_XDCS = 16;   // Data Chip Select / BSYNC pin
const int VS_DREQ = 4;    // Data Request pin
const int VS_RESET = 17;  // Reset pin (active low)

// HX710B control pins
const int HX_DOUT = 34;  // Data Out pin
const int HX_CLK = 33;   // Clock pin

// Speaker control pin
const int RELAY_INPUT = 32;  // Relay input pin

// Menus
const int NUM_MENU = 6;         // Number of menus
const int MAX_MENU_LENGTH = 20; // Maximum characters for the menu names

// Operating modes
const int PLAY_MODE = 0;       // Play mode
const int PATCH_MODE = 1;      // Patch selection mode
const int TUNE_MODE = 2;       // Tuning mode
const int RESIST_MODE = 3;     // Blow resistance mode
const int PRACTICE_MODE = 4;   // Fingering preactice mode
const int BLUETOOTH_MODE = 5;  // Bluetooth mode
const int MENU_MODE = 6;       // Menu mode

// Transpose ranges
const int MIN_TUNE = -5;
const int MAX_TUNE = 6;

// Key transpose letter to displays
char* KEY_TUNE[] = {
  "(G)",   // -5
  "(Ab)",  // -4
  "(A)",   // -3
  "(Bb)",  // -2
  "(B)",   // -1
  "(C)",   // 0
  "(Db)",  // +1
  "(D)",   // +2
  "(Eb)",  // +3
  "(E)",   // +4
  "(F)",   // +5
  "(Gb)"   // +6
};

// Note letter according to MIDI note
char* KEY_LETTER[] = {
  "   (C3)  ",  // 48
  "(C#3/Db3)",  // 49
  "   (D3)  ",  // 50
  "(D#3/Eb3)",  // 51
  "   (E3)  ",  // 52
  "   (F3)  ",  // 53
  "(F#3/Gb3)",  // 54
  "   (G3)  ",  // 55
  "(G#3/Ab3)",  // 56
  "   (A3)  "   // 57
  "(A#3/Bb3)",  // 58
  "   (B3)  ",  // 59
  "   (C4)  ",  // 60
  "(C#4/Db4)",  // 61
  "   (D4)  ",  // 62
  "(D#4/Eb4)",  // 63
  "   (E4)  ",  // 64
  "   (F4)  ",  // 65
  "(F#4/Gb4)",  // 66
  "   (G4)  ",  // 67
  "(G#4/Ab4)",  // 68
  "   (A4)  ",  // 69
  "(A#4/Bb4)",  // 70
  "   (B4)  ",  // 71
  "   (C5)  ",  // 72
  "(C#5/Db5)",  // 73
  "   (D5)  ",  // 74
  "(D#5/Eb5)",  // 75
  "   (E5)  ",  // 76
  "   (F5)  ",  // 77
  "(F#5/Gb5)",  // 78
  "   (G5)  ",  // 79
  "(G#5/Ab5)",  // 80
  "   (A5)  ",  // 81
  "(A#5/Bb5)",  // 82
  "   (B5)  ",  // 83
  "   (C6)  ",  // 84
  "(C#6/Db6)",  // 85
  "   (D6)  ",  // 86
  "(D#6/Eb6)",  // 87
  "   (E6)  ",  // 88
  "   (F6)  ",  // 89
  "(F#6/Gb6)",  // 90
  "   (G6)  ",  // 91
  "(G#6/Ab6)",  // 92
  "   (A6)  ",  // 93
  "(A#6/Bb6)",  // 94
  "   (B6)  ",  // 95
  "   (C7)  ",  // 96
};

// Plugin to put VS1053 into realtime MIDI mode
const unsigned short sVS1053b_Realtime_MIDI_Plugin[28] = {
  /* Compressed plugin */
  0x0007,
  0x0001,
  0x8050,
  0x0006,
  0x0014,
  0x0030,
  0x0715,
  0xb080, /*    0 */
  0x3400,
  0x0007,
  0x9255,
  0x3d00,
  0x0024,
  0x0030,
  0x0295,
  0x6890, /*    8 */
  0x3400,
  0x0030,
  0x0495,
  0x3d00,
  0x0024,
  0x2908,
  0x4d40,
  0x0030, /*   10 */
  0x0200,
  0x000a,
  0x0001,
  0x0050,
};
