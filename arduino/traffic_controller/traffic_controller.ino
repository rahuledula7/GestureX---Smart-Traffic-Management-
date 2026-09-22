#include <Wire.h>


// =====================================================
// TRAFFIC LIGHT PINS
// =====================================================

// EAST
const int E_RED = 2;
const int E_YELLOW = 3;
const int E_GREEN = 4;

// SOUTH
const int S_RED = 5;
const int S_YELLOW = 6;
const int S_GREEN = 7;

// WEST
const int W_RED = 8;
const int W_YELLOW = 9;
const int W_GREEN = 10;

// NORTH
const int N_RED = 11;
const int N_YELLOW = 12;
const int N_GREEN = 13;


// =====================================================
// I2C
// =====================================================

#define SENSOR_ADDRESS 8

byte northDensity = 0;
byte southDensity = 0;
byte eastDensity = 0;
byte westDensity = 0;


// =====================================================
// TIMING
// =====================================================

const unsigned long MIN_GREEN_TIME = 5000;
const unsigned long MAX_GREEN_TIME = 15000;

const unsigned long YELLOW_TIME = 2000;
const unsigned long ALL_RED_TIME = 1000;


// =====================================================
// PHASE
// =====================================================

enum Phase {
  NS_PHASE,
  EW_PHASE
};

Phase currentPhase;


// =====================================================
// OVERRIDE
// =====================================================

bool overrideActive = false;

enum OverrideDirection {
  NO_OVERRIDE,
  OVERRIDE_EW,
  OVERRIDE_NS
};

OverrideDirection overrideDirection = NO_OVERRIDE;


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(9600);

  Wire.begin();

  pinMode(E_RED, OUTPUT);
  pinMode(E_YELLOW, OUTPUT);
  pinMode(E_GREEN, OUTPUT);

  pinMode(S_RED, OUTPUT);
  pinMode(S_YELLOW, OUTPUT);
  pinMode(S_GREEN, OUTPUT);

  pinMode(W_RED, OUTPUT);
  pinMode(W_YELLOW, OUTPUT);
  pinMode(W_GREEN, OUTPUT);

  pinMode(N_RED, OUTPUT);
  pinMode(N_YELLOW, OUTPUT);
  pinMode(N_GREEN, OUTPUT);


  setAllRed();

  Serial.println("ARDUINO 1 READY");

  delay(1000);

  currentPhase = NS_PHASE;

  setNSGreen();

  Serial.println("NORTH + SOUTH GREEN");
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop() {

  checkSerialCommand();


  if (overrideActive) {

    if (overrideDirection == OVERRIDE_EW) {
      setEWGreen();
    }

    else if (overrideDirection == OVERRIDE_NS) {
      setNSGreen();
    }

    delay(100);

    return;
  }


  if (currentPhase == NS_PHASE) {
    runNSPhase();
  }

  else {
    runEWPhase();
  }
}


// =====================================================
// NS ADAPTIVE PHASE
// =====================================================

void runNSPhase() {

  unsigned long phaseStart = millis();


  while (true) {

    checkSerialCommand();

    if (overrideActive)
      return;


    readSensorData();


    byte nsDensity =
      northDensity + southDensity;

    byte ewDensity =
      eastDensity + westDensity;


    unsigned long elapsed =
      millis() - phaseStart;


    Serial.print("NS: ");
    Serial.print(nsDensity);

    Serial.print(" | EW: ");
    Serial.print(ewDensity);

    Serial.print(" | TIME: ");
    Serial.println(elapsed / 1000);


    if (elapsed < MIN_GREEN_TIME) {

      delay(300);

      continue;
    }


    if (ewDensity > nsDensity) {

      break;
    }


    if (elapsed >= MAX_GREEN_TIME) {

      break;
    }


    delay(300);
  }


  // SAFE TRANSITION

  setAllYellow();

  delay(YELLOW_TIME);

  setAllRed();

  delay(ALL_RED_TIME);


  currentPhase = EW_PHASE;

  setEWGreen();

  Serial.println("EAST + WEST GREEN");
}


// =====================================================
// EW ADAPTIVE PHASE
// =====================================================

void runEWPhase() {

  unsigned long phaseStart = millis();


  while (true) {

    checkSerialCommand();

    if (overrideActive)
      return;


    readSensorData();


    byte nsDensity =
      northDensity + southDensity;

    byte ewDensity =
      eastDensity + westDensity;


    unsigned long elapsed =
      millis() - phaseStart;


    Serial.print("EW: ");
    Serial.print(ewDensity);

    Serial.print(" | NS: ");
    Serial.print(nsDensity);

    Serial.print(" | TIME: ");
    Serial.println(elapsed / 1000);


    if (elapsed < MIN_GREEN_TIME) {

      delay(300);

      continue;
    }


    if (nsDensity > ewDensity) {

      break;
    }


    if (elapsed >= MAX_GREEN_TIME) {

      break;
    }


    delay(300);
  }


  // SAFE TRANSITION

  setAllYellow();

  delay(YELLOW_TIME);

  setAllRed();

  delay(ALL_RED_TIME);


  currentPhase = NS_PHASE;

  setNSGreen();

  Serial.println("NORTH + SOUTH GREEN");
}


// =====================================================
// NORTH + SOUTH GREEN
// =====================================================

void setNSGreen() {

  setAllRed();


  // NORTH GREEN
  digitalWrite(N_RED, LOW);
  digitalWrite(N_YELLOW, LOW);
  digitalWrite(N_GREEN, HIGH);


  // SOUTH GREEN
  digitalWrite(S_RED, LOW);
  digitalWrite(S_YELLOW, LOW);
  digitalWrite(S_GREEN, HIGH);


  // EAST RED
  digitalWrite(E_RED, HIGH);
  digitalWrite(E_YELLOW, LOW);
  digitalWrite(E_GREEN, LOW);


  // WEST RED
  digitalWrite(W_RED, HIGH);
  digitalWrite(W_YELLOW, LOW);
  digitalWrite(W_GREEN, LOW);
}


// =====================================================
// EAST + WEST GREEN
// =====================================================

void setEWGreen() {

  setAllRed();


  // EAST GREEN
  digitalWrite(E_RED, LOW);
  digitalWrite(E_YELLOW, LOW);
  digitalWrite(E_GREEN, HIGH);


  // WEST GREEN
  digitalWrite(W_RED, LOW);
  digitalWrite(W_YELLOW, LOW);
  digitalWrite(W_GREEN, HIGH);


  // NORTH RED
  digitalWrite(N_RED, HIGH);
  digitalWrite(N_YELLOW, LOW);
  digitalWrite(N_GREEN, LOW);


  // SOUTH RED
  digitalWrite(S_RED, HIGH);
  digitalWrite(S_YELLOW, LOW);
  digitalWrite(S_GREEN, LOW);
}


// =====================================================
// ALL YELLOW
// =====================================================

void setAllYellow() {

  digitalWrite(E_GREEN, LOW);
  digitalWrite(S_GREEN, LOW);
  digitalWrite(W_GREEN, LOW);
  digitalWrite(N_GREEN, LOW);


  digitalWrite(E_RED, LOW);
  digitalWrite(S_RED, LOW);
  digitalWrite(W_RED, LOW);
  digitalWrite(N_RED, LOW);


  digitalWrite(E_YELLOW, HIGH);
  digitalWrite(S_YELLOW, HIGH);
  digitalWrite(W_YELLOW, HIGH);
  digitalWrite(N_YELLOW, HIGH);
}


// =====================================================
// ALL RED
// =====================================================

void setAllRed() {

  digitalWrite(E_GREEN, LOW);
  digitalWrite(S_GREEN, LOW);
  digitalWrite(W_GREEN, LOW);
  digitalWrite(N_GREEN, LOW);


  digitalWrite(E_YELLOW, LOW);
  digitalWrite(S_YELLOW, LOW);
  digitalWrite(W_YELLOW, LOW);
  digitalWrite(N_YELLOW, LOW);


  digitalWrite(E_RED, HIGH);
  digitalWrite(S_RED, HIGH);
  digitalWrite(W_RED, HIGH);
  digitalWrite(N_RED, HIGH);
}


// =====================================================
// READ ARDUINO 2
// =====================================================

void readSensorData() {

  Wire.requestFrom(SENSOR_ADDRESS, 4);


  if (Wire.available() == 4) {

    northDensity = Wire.read();
    southDensity = Wire.read();
    eastDensity = Wire.read();
    westDensity = Wire.read();

  }

  else {

    Serial.println("I2C ERROR");
  }
}


// =====================================================
// PYTHON COMMANDS
// =====================================================

void checkSerialCommand() {

  if (Serial.available()) {

    String command =
      Serial.readStringUntil('\n');

    command.trim();


    // INDEX FINGER
    if (command == "EW") {

      activateEWOverride();
    }


    // OPEN PALM
    else if (command == "NS") {

      activateNSOverride();
    }


    // RETURN AUTOMATIC
    else if (command == "AUTO") {

      returnToAuto();
    }
  }
}


// =====================================================
// EAST + WEST OVERRIDE
// =====================================================

void activateEWOverride() {

  if (!overrideActive ||
      overrideDirection != OVERRIDE_EW) {

    Serial.println("OVERRIDE: EAST + WEST");


    setAllYellow();

    delay(YELLOW_TIME);


    setAllRed();

    delay(ALL_RED_TIME);


    setEWGreen();


    overrideActive = true;

    overrideDirection = OVERRIDE_EW;
  }
}


// =====================================================
// NORTH + SOUTH OVERRIDE
// =====================================================

void activateNSOverride() {

  if (!overrideActive ||
      overrideDirection != OVERRIDE_NS) {

    Serial.println("OVERRIDE: NORTH + SOUTH");


    setAllYellow();

    delay(YELLOW_TIME);


    setAllRed();

    delay(ALL_RED_TIME);


    setNSGreen();


    overrideActive = true;

    overrideDirection = OVERRIDE_NS;
  }
}


// =====================================================
// RETURN TO AUTO
// =====================================================

void returnToAuto() {

  if (overrideActive) {

    Serial.println("RETURNING TO AUTO");


    setAllYellow();

    delay(YELLOW_TIME);


    setAllRed();

    delay(ALL_RED_TIME);


    overrideActive = false;

    overrideDirection = NO_OVERRIDE;


    readSensorData();


    byte nsDensity =
      northDensity + southDensity;

    byte ewDensity =
      eastDensity + westDensity;


    if (ewDensity > nsDensity) {

      currentPhase = EW_PHASE;

      setEWGreen();

      Serial.println("AUTO: EAST + WEST GREEN");
    }

    else {

      currentPhase = NS_PHASE;

      setNSGreen();

      Serial.println("AUTO: NORTH + SOUTH GREEN");
    }
  }
}