//****************************
//  Functions
//****************************
// Display screen
void changeDisplay() {
  display.clearDisplay();  // Clear display
  switch (screen) {
    case 0:
      displayDefault();  // Default screen
      break;
    case 1:
      displayPatch();  // Select patch screen
      break;
    case 2:
      displayTune();  // Tuning screen
      break;
  }
  display.display();  // Display screen
}

// Default screen
void displayDefault() {
  // displayChanged = true;
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE, SH110X_BLACK);  // White text on black background
  display.setCursor(23, 18);
  display.write("PLAYING");
  display.setTextSize(1);
  display.setCursor(55, 40);
  display.write("MODE");
}

// Select patch screen
void displayPatch() {
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE, SH110X_BLACK);
  display.setCursor(35, 10);
  display.write("PATCH");

  // Display patch number
  if (patch >= 100)
    display.setCursor(45, 33);
  else if (patch >= 10)
    display.setCursor(55, 33);
  else
    display.setCursor(60, 33);

  display.print(patch);
}

// Tuning screen
void displayTune() {
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE, SH110X_BLACK);
  display.setCursor(28, 10);
  display.write("TUNING");

  // Display tuning value
  if (tuning > 0) {
    display.setCursor(33, 33);
    display.print("+");
    display.print(tuning);
  } else if (tuning < 0) {
    display.setCursor(33, 33);
    display.print(tuning);
  } else {
    display.setCursor(45, 33);
    display.print("0");
  }

  // Display tuning letter
  display.setCursor(65, 33);
  display.println(KEY_TUNE[tuning + 5]);
}

// Handle patch change
void patchChange() {
  patch = analogRead(PATCH_POT);
  patch = constrain(map(patch, 0, 1023, 60, 80), 0, 127);
  talkMIDI(0xB0, 0x00, 0x00);   // Select default bank GM1
  talkMIDI(0xC0, patch, 0x00);  // Patch number
  displayChanged = true;        // Update display
}

// Handle tuning key
void transpose() {
  tuning = analogRead(TUNE_POT);
  tuning = map(tuning, 0, 1023, MIN_TUNE, MAX_TUNE);  // Tune in range of 12 keys
  displayChanged = true;        // Update display
}

// Read pressure sensor and convert into breath level
void readPressure() {
  lastRawPressure = rawPressure;  // Save last raw pressure value for average
  while (!airPress.isReady())     // Wait for sensor to be ready
    ;
  airPress.readAndSelectNextData(HX710_DIFFERENTIAL_INPUT_40HZ);  // 40Hz input rate
  rawPressure = airPress.getLastDifferentialInput();              // Read sensor
  breath = (lastRawPressure + rawPressure) / 2;                   // Average pressure values
  breath = map(breath, MIN_PRESSURE, MAX_PRESSURE, 0, 127);       // convert to breath level
  breath = constrain(breath, 0, 127);                             // Constrain value within MIDI ranges

  if (breath <= ON_THRESH) {  // breath not enough to trigger sound
    breath = 0;
  }
}

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
  LHf = !digitalReadDebounce(22);
  LH1 = !digitalReadDebounce(23);
  LHb = !digitalReadDebounce(24);
  LH2 = !digitalReadDebounce(25);
  LH3 = !digitalReadDebounce(26);
  LHs1 = !digitalReadDebounce(27);
  LHs2 = !digitalReadDebounce(28);
  LHs3 = !digitalReadDebounce(29);
  Oct = !digitalReadDebounce(30);
  LHp1 = !digitalReadDebounce(31);
  LHp2 = !digitalReadDebounce(32);
  LHp3 = !digitalReadDebounce(33);
  RH1 = !digitalReadDebounce(34);
  RH2 = !digitalReadDebounce(35);
  RH3 = !digitalReadDebounce(36);
  RHs = !digitalReadDebounce(37);
  RHp1 = !digitalReadDebounce(38);
  RHp2 = !digitalReadDebounce(39);
  modeSW = digitalReadDebounce(MODE_SWITCH);
}

// Send MIDI data
void sendMIDI(byte data) {
  SPI.transfer(0);
  SPI.transfer(data);
}

// Send MIDI message
void talkMIDI(byte cmd, byte data1, byte data2) {
  // Wait for chip to be ready
  while (!digitalRead(VS_DREQ))
    ;
  digitalWrite(VS_XDCS, LOW);
  sendMIDI(cmd);
  // All commands less than 0xBn have 2 data bytes
  if ((cmd & 0xF0) <= 0xB0 || (cmd & 0xF0) >= 0xE0) {
    sendMIDI(data1);
    sendMIDI(data2);
  } else {
    sendMIDI(data1);
  }
  digitalWrite(VS_XDCS, HIGH);
}

// Send MIDI note-on message
void noteOn(byte channel, byte note, byte velocity) {
  talkMIDI((0x90 | channel), note, velocity);
}

// Send MIDI note-off message
void noteOff(byte channel, byte note, byte velocity) {
  talkMIDI((0x80 | channel), note, velocity);
}

// Startup animation 1
void startup1() {
  int i, j, k;

  display.clearDisplay();  // Clear display buffer
  display.display();

  for (i = 64; i >= 0; i -= 4) {  // i = top-left corner
    display.setTextColor(SH110X_WHITE, SH110X_BLACK);
    display.setTextSize(3);
    display.setCursor(12, 10);
    display.write("ESP-23");
    display.setTextSize(1);
    display.setCursor(5, 40);
    display.write("Electronic Saxophone");
    display.setCursor(42, 50);
    display.write("23-Keys");

    for (k = 0; k < i; k += 1)  // k = top-left corners
    {
      j = 128 - (k * 2);                                   // j = box width
      display.drawRect(k, k / 2, j, j / 2, SH110X_BLACK);  // drawRect(x, y, width, height, color)
    }
    display.display();
  }
}

// Clear startup animation
void startup2() {
  int i, j;

  for (i = 64; i >= 0; i -= 4) {                  // i = top-left corner
    j = 128 - i * 2;                              // j = box width
    display.fillRect(i, 0, j, 64, SH110X_BLACK);  // fillRect(x, y, width, height, color)
    display.display();
  }
}

// Startup sound
void startSound() {
  // Select patch
  talkMIDI(0xB0, 0x00, 0x00);  // Default bank GM1
  talkMIDI(0xC0, 74, 0x00);    // Patch number

  // Play note from C4 to C5
  for (int i = C4_NOTE; i < C5_NOTE; i++) {
    noteOn(MIDIchannel, i, 70);
    delay(200);
    noteOff(MIDIchannel, i, 0);
  }
}

// Use write VS1053 register
void VSWriteRegister(unsigned char addressbyte, unsigned char highbyte, unsigned char lowbyte){
  while(!digitalRead(VS_DREQ)) ; //Wait for DREQ to go high indicating IC is available
  digitalWrite(VS_XCS, LOW); //Select control

  //SCI consists of instruction byte, address byte, and 16-bit data word.
  SPI.transfer(0x02); //Write instruction
  SPI.transfer(addressbyte);
  SPI.transfer(highbyte);
  SPI.transfer(lowbyte);
  while(!digitalRead(VS_DREQ)) ; //Wait for DREQ to go high indicating command is complete
  digitalWrite(VS_XCS, HIGH); //Deselect Control
}

// Use to put VS1053 into realtime MIDI mode (only first time)
void VSLoadUserCode() {
  int i = 0;

  while (i<sizeof(sVS1053b_Realtime_MIDI_Plugin)/sizeof(sVS1053b_Realtime_MIDI_Plugin[0])) {
    unsigned short addr, n, val;
    addr = sVS1053b_Realtime_MIDI_Plugin[i++];
    n = sVS1053b_Realtime_MIDI_Plugin[i++];
    while (n--) {
      val = sVS1053b_Realtime_MIDI_Plugin[i++];
      VSWriteRegister(addr, val >> 8, val & 0xFF);
    }
  }
}