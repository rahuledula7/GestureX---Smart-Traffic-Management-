#include <Wire.h>

#define SLAVE_ADDRESS 8

// IR SENSOR PINS
const int N1 = 2;
const int N2 = 3;

const int S1 = 4;
const int S2 = 5;

const int E1 = 6;
const int E2 = 7;

const int W1 = 8;
const int W2 = 9;

byte northDensity = 0;
byte southDensity = 0;
byte eastDensity = 0;
byte westDensity = 0;


void setup() {

  Serial.begin(9600);

  pinMode(N1, INPUT);
  pinMode(N2, INPUT);

  pinMode(S1, INPUT);
  pinMode(S2, INPUT);

  pinMode(E1, INPUT);
  pinMode(E2, INPUT);

  pinMode(W1, INPUT);
  pinMode(W2, INPUT);

  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(sendData);

  Serial.println("ARDUINO 2 - SENSOR + I2C READY");
}


void loop() {

  northDensity = getDensity(N1, N2);
  southDensity = getDensity(S1, S2);

  eastDensity = getDensity(E1, E2);
  westDensity = getDensity(W1, W2);


  Serial.print("N: ");
  Serial.print(northDensity);

  Serial.print(" | S: ");
  Serial.print(southDensity);

  Serial.print(" | E: ");
  Serial.print(eastDensity);

  Serial.print(" | W: ");
  Serial.println(westDensity);

  delay(300);
}


byte getDensity(int nearSensor, int farSensor) {

  bool nearDetected =
    (digitalRead(nearSensor) == LOW);

  bool farDetected =
    (digitalRead(farSensor) == LOW);


  if (!nearDetected && !farDetected)
    return 0;

  if (nearDetected && !farDetected)
    return 1;

  if (nearDetected && farDetected)
    return 2;

  return 1;
}


void sendData() {

  Wire.write(northDensity);
  Wire.write(southDensity);
  Wire.write(eastDensity);
  Wire.write(westDensity);
}