//**********************************************************
//  Functions
//**********************************************************
// Display data
void changeDisplay() {
  display.clearBuffer();  // clear buffer for storing display content in RAM
  // display.clearDisplay();  // Clear display
  switch (mode) {
    case MENU_MODE:
      displayMenu();
      break;
    case PLAY_MODE:
      displayPlaying();  // Playing screen
      break;
    case PATCH_MODE:
      displayPatch();  // Patch selection screen
      break;
    case TUNE_MODE:
      displayTune();  // Tuning screen
      break;
    case RESIST_MODE:  // Breath resistance screen
      displayResist();
      break;
    case PRACTICE_MODE:
      displayFingering();  // Fingering practice screen
      break;
    case BLUETOOTH_MODE:
      displayBluetooth();  // Bluetooth screen
      break;
  }
  display.sendBuffer();  // Display screen
}

// Menu selection screen
void displayMenu() {
  // selected item background
  display.drawXBMP(0, 22, 128, 21, selectedBackground);

  display.setFont(u8g_font_7x14);
  // draw previous menu icon and label
  display.drawStr(31, 16, menuName[previousMenu]);
  display.drawXBMP(4, 2, 16, 16, bitmap_icons[previousMenu]);

  // draw next menu icon and label
  display.drawStr(31, 58, menuName[nextMenu]);
  display.drawXBMP(4, 46, 16, 16, bitmap_icons[nextMenu]);

  // draw selected menu icon and label in bold text
  display.setFont(u8g_font_7x14B);
  display.drawStr(31, 36, menuName[selectedMenu]);
  display.drawXBMP(4, 24, 16, 16, bitmap_icons[selectedMenu]);

  // draw scrollbar background
  display.drawXBMP(120, 0, 8, 64, scrollbarBackground);

  // draw scrollbar handle
  display.drawBox(122, 64 / NUM_MENU * selectedMenu, 3, 64 / NUM_MENU);
}

// Display playing screen
void displayPlaying() {
  // Display background
  display.drawXBMP(0, 0, 128, 64, playModeBackground);
}

// Display patch selection screen
void displayPatch() {
  // Display background
  display.drawXBMP(0, 0, 128, 64, tunePatchBackground);
  display.setFont(u8g_font_7x14B);
  display.drawStr(46, 14, "Patch");

  // Display patch number
  display.setFont(u8g_font_7x14B);
  if (patch >= 100)
    display.setCursor(54, 34);
  else if (patch >= 10)
    display.setCursor(57, 34);
  else
    display.setCursor(60, 34);

  display.print(patch);
}

// Tuning screen
void displayTune() {
  // Display background
  display.drawXBMP(0, 0, 128, 64, tunePatchBackground);
  display.setFont(u8g_font_7x14B);
  display.drawStr(42, 16, "Tuning");

  // Display tuning value
  display.setFont(u8g_font_7x14B);
  if (tuning > 0) {
    display.setCursor(43, 35);
    display.print("+");
    display.print(tuning);
  } else if (tuning < 0) {
    display.setCursor(43, 35);
    display.print(tuning);
  } else {
    display.setCursor(45, 35);
    display.print("0");
  }

  // Display tuning letter
  display.drawStr(65, 35, KEY_TUNE[tuning + 5]);
}

// Breath resistance screen
void displayResist() {
  // Display background
  display.drawXBMP(0, 0, 128, 64, tunePatchBackground);
  display.setFont(u8g_font_7x14B);
  display.drawStr(29, 14, "Resistance");

  // Display resistance level
  display.setFont(u8g_font_7x14B);
if (resistLevel >= 10)
    display.setCursor(57, 34);
  else
    display.setCursor(60, 34);

  display.print(resistLevel);
}

// Fingering practice screen
void displayFingering() {
  display.drawXBMP(0, 0, 128, 64, fingeringBackground);
  display.setFont(u8g_font_7x14B);
  display.drawStr(31, 37, KEY_LETTER[pitch - 47]);
  display.setCursor(55, 37);
}

// Bluetooth screen
void displayBluetooth() {
  if (bleConnected == true) {
    display.drawXBMP(0, 0, 128, 64, connectedBackground);
  } else {
    display.drawXBMP(0, 0, 128, 64, pairingBackground);
  }
}

// Handle patch change
void patchChange() {
  patch = analogRead(CONTROL_POT);
  patch = constrain(map(patch, 0, 4095, 0, 127), 0, 127);
  talkMIDI(0xB0, 0x00, 0x00);   // Default bank GM1
  talkMIDI(0xC0, patch, 0x00);  // Patch number
  displayChanged = true;        // Update display
}

// Handle tuning key
void transpose() {
  tuning = analogRead(CONTROL_POT);
  tuning = map(tuning, 0, 4095, MIN_TUNE, MAX_TUNE);  // Tune in 12 keys range
  displayChanged = true;
}

// Handle breath resistance change
void adjustResist() {
  resistLevel = analogRead(CONTROL_POT);
  resistLevel = map(resistLevel, 0, 4095, 0, 20);  // Adjust in 20 levels
  blowResist = resistLevel * LEVEL_STEP;           // Step values per level
  displayChanged = true;
}

// Handle bluetooth connection
void bleStatus() {
  if (BLEMidiServer.isConnected()) {
    if (lastBleConnected == false) {
      bleConnected = true;
      lastBleConnected = true;
      displayChanged = true;
    }
  } else {
    bleConnected = false;
    if (lastBleConnected == true) {
      lastBleConnected = false;
      displayChanged = true;
    }
  }
}

void blePair() {
  if (isBleOn == false) {
    BLEMidiServer.begin("ESP-23");
    isBleOn = true;
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
    switchState = switchState;
  }
  lastSwitchState = switchState;
  return switchState;
}

// Startup animation 1
void startup1() {
  int i, j, k;

  display.clearBuffer();  // Clear display buffer

  for (i = 64; i >= 0; i -= 4) {  // i = top-left corner
    display.setDrawColor(1);      // set the color to white
    display.setFont(u8g_font_7x14B);
    display.setCursor(42, 28);
    display.print("ESP-23");
    display.setFont(u8g_font_6x10);
    display.setCursor(5, 40);
    display.print("Electronic Saxophone");
    display.setCursor(42, 55);
    display.print("23-Keys");

    display.setDrawColor(0);
    for (k = 0; k < i; k += 1)  // k = top-left corners
    {
      j = 128 - (k * 2);                      // j = box width
      display.drawFrame(k, k / 2, j, j / 2);  // drawRect(x, y, width, height, color)
    }
    display.sendBuffer();
  }
}

// Clear startup animation
void startup2() {
  int i, j;

  for (i = 64; i >= 0; i -= 4) {   // i = top-left corner
    j = 128 - i * 2;               // j = box width
    display.drawBox(i, 0, j, 64);  // fillRect(x, y, width, height, color)
    display.sendBuffer();
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
void VSWriteRegister(unsigned char addressbyte, unsigned char highbyte, unsigned char lowbyte) {
  while (!digitalRead(VS_DREQ))
    ;                         //Wait for DREQ to go high indicating IC is available
  digitalWrite(VS_XCS, LOW);  //Select control

  //SCI consists of instruction byte, address byte, and 16-bit data word.
  SPI.transfer(0x02);  //Write instruction
  SPI.transfer(addressbyte);
  SPI.transfer(highbyte);
  SPI.transfer(lowbyte);
  while (!digitalRead(VS_DREQ))
    ;                          //Wait for DREQ to go high indicating command is complete
  digitalWrite(VS_XCS, HIGH);  //Deselect Control
}

// Use to put VS1053 into realtime MIDI mode (only first time)
void VSLoadUserCode() {
  int i = 0;

  while (i < sizeof(sVS1053b_Realtime_MIDI_Plugin) / sizeof(sVS1053b_Realtime_MIDI_Plugin[0])) {
    unsigned short addr, n, val;
    addr = sVS1053b_Realtime_MIDI_Plugin[i++];
    n = sVS1053b_Realtime_MIDI_Plugin[i++];
    while (n--) {
      val = sVS1053b_Realtime_MIDI_Plugin[i++];
      VSWriteRegister(addr, val >> 8, val & 0xFF);
    }
  }
}

// Send MIDI data
void sendMIDI(byte data) {
  SPI.transfer(0);
  SPI.transfer(data);
}

// Send MIDI message
void talkMIDI(byte cmd, byte data1, byte data2) {
  // Wait for chip to be ready (Unlikely to be an issue with real time MIDI)
  while (!digitalRead(VS_DREQ))
    ;
  digitalWrite(VS_XDCS, LOW);
  sendMIDI(cmd);
  // Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes
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
