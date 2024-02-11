//****************************
//  Variables
//****************************
HX710 airPress;  // Air pressure sensor

Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, -1);  // OLED Display

// Frame timer variables
unsigned long current100HzTime;
unsigned long previous100HzTime;
unsigned long current10HzTime;
unsigned long previous10HzTime;

int32_t rawPressure;      // Raw pressure value from sensor
int32_t lastRawPressure;  // Last pass pressure value use for average
int32_t breath;           // Breath value
int32_t breathResist;     // Breath resistance value
int newBreath;            // Breath value for tounged note
int lastBreath;           // Last Breath value
int pitch;                // Current pitch playing
int lastPitch;            // Last pitch played

int breathLevel;
int pressureSensor;  // Pressure data from breath sensor, for midi breath cc and breath threshold checks
int velocity;        // Remapped midi velocity from breath sensor
int activeVelocity;  // Playing velocity
int fingeredNote;    // Note calculated from fingering
int activeNote;      // Note playing
int tuning;          // Transpose key tuning
int patch;           // Patch number

int modeSW;           // Mode switch
int modeSwitchState;  // Mode switch state
int mode;             // Operating mode

int screen;           // Screens to display
bool displayChanged;  // Display has changed

// Key variables, TRUE (1) for pressed, FALSE (0) for not pressed
int switchState;      // Switch state use for debounce
int lastSwitchState;  // Last switch state use for debounce
int LHf;              // Left Hand high f key (pitch change +5)
int LH1;              // Left Hand key 1 (pitch change -2)
int LHb;              // Left Hand bis key (pitch change -1 unless both LH1 and LH2 are pressed)
int LH2;              // Left Hand key 2  (with LH1 also pressed pitch change is -2, otherwise -1)
int LH3;              // Left Hand key 3 (pitch change -2)
int Oct;              // Octave switch key (pitch change +12)
int LHs1;             // Left Hand side key 1 (pitch change +1)
int LHs2;             // Left Hand side key 3 (pitch change +1)
int LHs3;             // Left Hand side key 3 (pitch change +1)
int LHp1;             // Left Hand pinky key 1 (pitch change +1)
int LHp2;             // Left Hand pinky key 2 (pitch change -1)
int LHp3;             // Left Hand pinky key 3 (pitch change -2)
int RH1;              // Right Hand key 1 (with LH3 also pressed pitch change is -2, otherwise -1)
int RH2;              // Right Hand key 2 (pitch change -1)
int RH3;              // Right Hand key 3 (pitch change -2)
int RHs;              // Right Hand side key  (pitch change +1)
int RHp1;             // Right Hand pinky key 1 (pitch change +1)
int RHp2;             // Right Hand pinky key 2 (pitch change -2)