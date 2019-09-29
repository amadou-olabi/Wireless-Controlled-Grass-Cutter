#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#define RIGHT "RIGHT"
#define LEFT "LEFT"
#define UP "UP"
#define DOWN "DOWN"
#define STATIC "STATIC"
#define enA 2   // Note: Pin 9 in previous video ( pin 10 is used for the SPI communication of the NRF24L01)
#define in1 4
#define in2 5
#define enB 3   // Note:  Pin 10 in previous video
#define in3 6
#define in4 7

// Arduino pin numbers
RF24 radio(8, 9); // CE, CSN
const byte address[6] = "00001";
char Position[32] = "";
int motorSpeedA = 0;
int motorSpeedB = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  digitalWrite(enA, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(enB, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (radio.available()) {
    radio.read(&Position, sizeof(Position)); // Read the data and put it into character array
  }
  Serial.println(Position);
  moveDevice();
  strcpy(Position, "");
  delay(10);
}

void moveDevice() {
  int key;
  if (strcmp(Position, UP) == 0) key = 1;
  else if (strcmp(Position, DOWN) == 0) key = 2;
  else if (strcmp(Position, LEFT) == 0) key = 3;
  else if (strcmp(Position, RIGHT) == 0) key = 4;
  else if (strcmp(Position, STATIC) == 0) key = 5;
  else key = 0;

  switch (key) {
    case 1:
      forward();
      break;
    case 2:
      backward();
      break;
    case 3:
      left();
      break;
    case 4:
      right();
      break;
    case 5:
    case 0:
      fixedP();
      break;
  }
  if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }
  analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
  analogWrite(enB, motorSpeedB); // Send PWM signal to motor B
}

void forward() {
  //  digitalWrite(in1, HIGH);
  //  digitalWrite(in2, LOW);
  //
  //  digitalWrite(in3, HIGH);
  //  digitalWrite(in4, LOW);

  // Set Motor A forward
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  // Set Motor B forward
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  int vert = 1023;

  motorSpeedA = map(vert, 564, 1023, 0, 255);
  motorSpeedB = map(vert, 564, 1023, 0, 255);
}
void backward() {
  //  digitalWrite(in1, LOW);
  //  digitalWrite(in2, HIGH);
  //
  //  digitalWrite(in3, LOW);
  //  digitalWrite(in4, HIGH);

  // Set Motor A backward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // Set Motor B backward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  int vert = 0;

  vert -= 460;
  vert *= -1;

  motorSpeedA = map(vert, 0, 460, 0, 255);
  motorSpeedB = map(vert, 0, 460, 0, 255);
}
void left() {
  int horiz = 0;
  horiz -= 460;
  horiz *= -1;

  int horizMapped = map(horiz, 460, 0, 0, 255);

  motorSpeedA -= horizMapped;
  motorSpeedB += horizMapped;

  if (motorSpeedA < 0)
    motorSpeedA = 0;
  if (motorSpeedB > 255)
    motorSpeedB = 255;
}
void right() {
  int horiz = 1023;
  int horizMapped = map(horiz, 564, 1023, 0, 255);

  motorSpeedA += horizMapped;
  motorSpeedB -= horizMapped;

  if (motorSpeedA > 255)
    motorSpeedA = 255;
  if (motorSpeedB < 0)
    motorSpeedB = 0;
}
void fixedP() {
  motorSpeedA = 0;
  motorSpeedB = 0;
}
