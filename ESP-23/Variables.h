//**********************************************************
//  Variables
//**********************************************************
HX710 airPress;  // Air pressure sensor

// Adafruit_SH1106G display(128, 64, &Wire, -1);  // OLED Display
U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

unsigned long currentProgramTime;   // Current program running time
unsigned long previousProgramTime;  // Previous program running time
unsigned long currentDisplayTime;   // Current display updating time
unsigned long previousDisplayTime;  // Previous display updating time

int32_t rawPressure;      // Raw pressure value from sensor
int32_t lastRawPressure;  // Last pass pressure value use for average
int32_t breath;           // Breath value
int32_t blowResist;       // Blow resistance value
int resistLevel;          // Blow resistance level
int newBreath;            // Breath value for tounged note
int lastBreath;           // Last Breath value
int midiNote;             // MIDI note from fingering
int pitch;                // Current pitch playing
int lastPitch;            // Last pitch played
int tuning;               // Transpose key tuning
int patch;                // Patch number

int buttonSelectState;  // Select button state
int buttonUpState;      // Up button state
int buttonDownState;    // Down button state
int mode;               // Operating mode

int selectedMenu;     // Current selected menu
int previousMenu;     // Previous menu to display
int nextMenu;         // Next menu to display
bool displayChanged;  // Display has been changed

int switchState;      // Switch state use for debounce
int lastSwitchState;  // Last switch state use for debounce

bool lastBleConnected;  // BLE connection status
bool bleConnected;
bool isBleOn;