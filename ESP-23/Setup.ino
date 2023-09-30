//**********************************************************
//  Setup
//**********************************************************
void setup() {

  // Instrument configuration pins
  pinMode(CONTROL_POT, INPUT);           // Control potentiometer
  pinMode(SELECT_BUTTON, INPUT_PULLUP);  // Mode select
  pinMode(UP_BUTTON, INPUT_PULLUP);  // Mode Scroll up
  pinMode(DOWN_BUTTON, INPUT_PULLUP);  // Mode Scroll down

  // Speaker control pin
  pinMode(RELAY_INPUT, OUTPUT); // Speaker enable
  digitalWrite(RELAY_INPUT, LOW); // Enable speaker

  // VS1053 control pins
  pinMode(VS_DREQ, INPUT);      // Data Request
  pinMode(VS_XCS, OUTPUT);      // Chip Select
  pinMode(VS_XDCS, OUTPUT);     // Data Chip Select
  digitalWrite(VS_XCS, HIGH);   // Deselect Control
  digitalWrite(VS_XDCS, HIGH);  // Deselect Data
  pinMode(VS_RESET, OUTPUT);    // Reset

  // Use serial for reading switches from another board
  Serial.begin(115200);  // Start serial communication

  // Initialize VS1053 chip
  digitalWrite(VS_RESET, LOW);  // Put VS1053 into hardware reset

  // Setup SPI for VS1053
  pinMode(10, OUTPUT);  // Pin 10 must be set as an output for the SPI communication to work
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV16);  // Set SPI bus speed to 1MHz (16MHz / 16 = 1MHz)
  SPI.transfer(0xFF);                    // Throw a dummy byte at the bus
  delayMicroseconds(1);
  digitalWrite(VS_RESET, HIGH);  // Bring up VS1053

  VSLoadUserCode();  // Use to put VS1053 into realtime MIDI mode (only first time)

  airPress.initialize(HX_CLK, HX_DOUT);  // Initialize HX710B chip

  // Initialize variables
  lastBreath = 0;
  lastPitch = 0;
  tuning = 0;
  mode = PLAY_MODE;
  selectedMenu = PLAY_MODE;
  previousMenu = BLUETOOTH_MODE;
  nextMenu = PATCH_MODE;
  displayChanged = true;
  lastBleConnected = false;
  isBleOn = false;

  // Initialize display and play startup sound
  display.begin();
  display.setBitmapMode(1);
  startup1();
  startSound();
  delay(1000);
  startup2();
  display.setColorIndex(1);  // set the color to white

}
