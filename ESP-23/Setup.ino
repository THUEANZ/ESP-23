//****************************
//  Setup
//****************************
void setup() {
  for (int i = 0; i < NUM_SWITCHES; i++) {  // Set inputs with pull-up
    pinMode(switchPins[i], INPUT_PULLUP);
  }

  pinMode(PATCH_POT, INPUT);           // Patch select potentiometer
  pinMode(TUNE_POT, INPUT);           // Transpose select potentiometer
  pinMode(MODE_SWITCH, INPUT_PULLUP);  // Mode Select

  pinMode(VS_DREQ, INPUT);
  pinMode(VS_XCS, OUTPUT);
  pinMode(VS_XDCS, OUTPUT);
  digitalWrite(VS_XCS, HIGH);   // Deselect Control
  digitalWrite(VS_XDCS, HIGH);  // Deselect Data
  pinMode(VS_RESET, OUTPUT);
  Serial.begin(115200);  // Use serial for debugging

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

  //VSLoadUserCode(); // Use to put VS1053 into realtime MIDI mode (only first time)

  airPress.initialize(HX_CLK, HX_DOUT);  // Initialize HX710B chip

  // Initialize variables
  lastBreath = 0;
  lastPitch = 0;
  tuning = 0;
  mode = PLAY_MODE;
  screen = 0;

  // Initialize display
  display.begin(0x3C, true);
  startup1();
  startSound();
  startup2();
  
}