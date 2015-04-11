#include "Wire.h"

// Gate 1 Up
const int light = 13;    // testing

const int turntable1 = A3;
const int turntable2 = A2;

const int eastgatelimit = A0;    // button 1
const int westgatelimit = A1;    // button 2

const int eastgate1 = 2;    // button 3
const int eastgate2 = 3;    // button 4

const int westgate1 = 0;    // button 5
const int westgate2 = 1;    // button 6

const int emstop = 4;    // Really necessary?  Just user power.
const int maxcom = 5;

//Bytes and Shit
byte addrB1 = 0x0B;    // first grizzly address - Spinner 
byte addrB2 = 0x0E;    // second grizzly address - Gate1
byte addrB3 = 0x0F;    // third grizzly address - Gate2


byte currLimitSpinner[3] = {0b00010011, 54, 0};
byte startB[8] = {0b00010011,0x0,0x0,0x0,0x0,100,0x0,0x0};
byte startBSlow[8] = {0b00010011,0x0,0x0,0x0,0x0,30,0x0,0x0};
byte startBMedium[8] = {0b00010011,0x0,0x0,0x0,0x0,75,0x0,0x0};
byte stopB[8] = {0b00010011,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
byte reverseB[8] = {0b00010011,0x0,0x0,0x0,0x0,0x9C,0xFF,0x0};
byte reverseBSlow[8] = {0b00010011,0x0,0x0,0x0,0x0,0xE2,0xFF,0x0};
byte reverseBMedium[8] = {0b00010011,0x0,0x0,0x0,0x0,0x8B,0xFF,0x0};
byte noaccel[2] = {0b00010011, 20};
byte setToutB[2] = {0x0, 0x08};

const int ledPin = 13;      // the number of the LED pin
int buttonState = 0;         // variable for reading the pushbutton status
#define ON LOW
#define OFF HIGH

unsigned long time;
unsigned long gate1timer;    // Lower after 8 seconds
unsigned long gate2timer;
unsigned long spinnertimer;

boolean dir = true;


void setup() {
  //Serial.begin(9600);
  //Serial.println("Started");
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  
  // Initialize I2C
  Wire.begin();
  Wire.beginTransmission(addrB1);
  Wire.write(0x90);
  Wire.write(noaccel,2);
  Wire.endTransmission();
  Wire.beginTransmission(addrB1);
  Wire.write(0x80);
  Wire.write(setToutB,2);
  Wire.endTransmission();
  
  Wire.beginTransmission(addrB2);
  Wire.write(0x90);
  Wire.write(noaccel,2);
  Wire.endTransmission();
  Wire.beginTransmission(addrB2);
  Wire.write(0x80);
  Wire.write(setToutB,2);
  Wire.endTransmission();
  
  Wire.beginTransmission(addrB3);
  Wire.write(0x90);
  Wire.write(noaccel,2);
  Wire.endTransmission();
  Wire.beginTransmission(addrB3);
  Wire.write(0x80);
  Wire.write(setToutB,2);
  Wire.endTransmission();
  
  time = millis();
  gate1timer=0;
  gate2timer=0;
  spinnertimer=0;

  // Initialize buttons
  //pinMode(light, INPUT);
  
  //digitalWrite(light, HIGH);
  pinMode(eastgate1, INPUT);
  digitalWrite(eastgate1, HIGH); 
  pinMode(eastgate2, INPUT);
  digitalWrite(eastgate2, HIGH); 
  
  pinMode(westgate1, INPUT);
  digitalWrite(westgate1, HIGH); 
  pinMode(westgate2, INPUT);
  digitalWrite(westgate2, HIGH); 

  pinMode(turntable1, INPUT);
  digitalWrite(turntable1, HIGH); 
  pinMode(turntable2, INPUT);
  digitalWrite(turntable2, HIGH);
  
  pinMode(eastgatelimit, INPUT);
  digitalWrite(eastgatelimit, HIGH); 
  pinMode(westgatelimit, INPUT);
  digitalWrite(westgatelimit, HIGH);
  
  pinMode(eastgatelimit, INPUT);
  digitalWrite(eastgatelimit, HIGH); 
  pinMode(westgatelimit, INPUT);
  digitalWrite(westgatelimit, HIGH);
  
  pinMode(emstop, INPUT);
  digitalWrite(emstop, HIGH); 
  pinMode(maxcom, INPUT);
  digitalWrite(maxcom, HIGH);
  
  // Serial.println("Started 2");
}

void loop() {
  /*Serial.print(digitalRead(buttonPin1));
  Serial.print(" ");
  Serial.print(digitalRead(buttonPin2));
  Serial.print(" ");
  Serial.print(digitalRead(buttonPin3));
  Serial.print(" ");
  Serial.print(digitalRead(buttonPin4));
  Serial.print(" ");
  Serial.print(digitalRead(buttonPin5));
  Serial.print(" ");
  Serial.print(digitalRead(buttonPin6));
  Serial.println(" ");*/
  // Test stuff
  if (digitalRead(light) == LOW) {
    digitalWrite(ledPin, LOW);
  }
  else {
    digitalWrite(ledPin, HIGH);
    //Serial.write("this light should be on");
  }
    
  time = millis();
  // East Gate
  if (digitalRead(eastgate1) == LOW || digitalRead(eastgate2) == LOW) {
      digitalWrite(ledPin, LOW);
      if (gate1timer < time) {
        gate1timer = time+10000;
      }
  }
  // Going up
  if ((time < gate1timer-9000) && (gate1timer > 0) && (digitalRead(eastgatelimit) != LOW)) {
    Wire.beginTransmission(addrB2);
    Wire.write(0x1);
    Wire.write(startBMedium,8);
    Wire.endTransmission();
  }
  // Going down
  else if ((time > gate1timer-1500) && (time < gate1timer)) {
    Wire.beginTransmission(addrB2);
    Wire.write(0x1);
    Wire.write(reverseBMedium,8);
    Wire.endTransmission();
  }
  // Stopped
  else {
    Wire.beginTransmission(addrB2);
    Wire.write(0x1);
    Wire.write(stopB,8);
    Wire.endTransmission();
  }

  // West Gate
  if (digitalRead(westgate1) == LOW || digitalRead(westgate2) == LOW) {
      digitalWrite(ledPin, LOW);
      if (gate2timer < time) {
        gate2timer = time+10000;
      }
  }
  // Going up
  if ((time < gate2timer-9000) && (gate2timer > 0) && (digitalRead(westgatelimit) != LOW)) {
    Wire.beginTransmission(addrB3);
    Wire.write(0x1);
    Wire.write(startBMedium,8);
    Wire.endTransmission();
  }
  // Going down
  else if ((time > gate2timer-1500) && (time < gate2timer)) {
    Wire.beginTransmission(addrB3);
    Wire.write(0x1);
    Wire.write(reverseBMedium,8);
    Wire.endTransmission();
  }
  // Stopped
  else {
    Wire.beginTransmission(addrB3);
    Wire.write(0x1);
    Wire.write(stopB,8);
    Wire.endTransmission();
  }

  // Spinner
  if(digitalRead(turntable1) == LOW || digitalRead(turntable2) == LOW) {
    if (spinnertimer < time) {
      dir = !dir;
      spinnertimer = time+5000;
    }
  }
  if (digitalRead(maxcom) == LOW) {
    Wire.beginTransmission(addrB1);
    Wire.write(0x1);
    Wire.write(stopB,8);
    Wire.endTransmission();
  }
  else if(dir) {
    //Serial.println("Forward");
    Wire.beginTransmission(addrB1);
    Wire.write(0x1);
    Wire.write(startBSlow,8);
    Wire.endTransmission();
  }
  else {
    //Serial.println("Backward");
    Wire.beginTransmission(addrB1);
    Wire.write(0x1);
    Wire.write(reverseBSlow,8);
    Wire.endTransmission();
  }
}
