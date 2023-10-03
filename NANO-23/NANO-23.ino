// #include <Wire.h>

const int DEBOUNCE_DELAY = 50;  // Delay before read actual switch values
const int START_NOTE = 73;      // MIDI start note (C#4)

unsigned long currentTime;
unsigned long previousTime;

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

int midiNote;  // MIDI note value

//****************************
//  Setup
//****************************
void setup() {
  for (int i = 2; i < 20; i++) {  // Pull-up switches
    pinMode(i, INPUT_PULLUP);
  }
  Serial.begin(115200);  // Start serial communication
  delay(2000);
}

//****************************
//  Main program loop
//****************************
void loop() {
  currentTime = millis();
  if ((currentTime - previousTime) >= 40) {
    previousTime = currentTime;

    readSwitches();  // Read each switch value

    // Calculate midi note from all pressed switches
    midiNote = START_NOTE + 5 * LHf - 2 * LH1 - (LHb && !(LH1 && LH2)) - LH2 - (LH2 && LH1) - 2 * LH3 + LHp1 - LHp2 - 2 * LHp3 - RH1 - (RH1 && LH3) - RH2 - 2 * RH3 + RHp1 - 2 * RHp2 + RHs + 12 * Oct;

    if (Serial.availableForWrite())
      Serial.write(midiNote);  // Send MIDI note through Serial
  }
}

//****************************
//  Functions
//****************************
// Function to read and debounce a switch
int digitalReadDebounce(int pin) {
  unsigned long lastDebounceTime = 0;
  int switchState = digitalRead(pin);

  if (switchState != lastSwitchState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (switchState != lastSwitchState) {
      lastSwitchState = switchState;
      return switchState;
    }
    return lastSwitchState;
  }
}

// Read switches and put inverted value in variables
void readSwitches() {
  LHf = !digitalReadDebounce(2);
  LH1 = !digitalReadDebounce(3);
  LHb = !digitalReadDebounce(4);
  LH2 = !digitalReadDebounce(5);
  LH3 = !digitalReadDebounce(6);
  LHs1 = !digitalReadDebounce(7);
  LHs2 = !digitalReadDebounce(8);
  LHs3 = !digitalReadDebounce(9);
  Oct = !digitalReadDebounce(10);
  LHp1 = !digitalReadDebounce(11);
  LHp2 = !digitalReadDebounce(12);
  LHp3 = !digitalReadDebounce(13);
  RH1 = !digitalReadDebounce(14);
  RH2 = !digitalReadDebounce(15);
  RH3 = !digitalReadDebounce(16);
  RHs = !digitalReadDebounce(17);
  RHp1 = !digitalReadDebounce(18);
  RHp2 = !digitalReadDebounce(19);
}

// Drop outdated data from serial buffer
void dropOutdated() {
  int bytesInBuffer = Serial.available();  // Get number of bytes in buffer
  // Loop through buffer and drop outdated datas
  for (int i = 0; i < bytesInBuffer; i++) {
    uint8_t data = Serial.read();
  }
}
