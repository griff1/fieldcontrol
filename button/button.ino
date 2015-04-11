#include "Wire.h"

// Gate 1 Up
const int light = 13;    // testing

const int buttonPin1 = A2;    // button 1
const int buttonPin2 = A1;    // button 2

const int buttonPin3 = 2;    // button 3
const int buttonPin4 = 3;    // button 4

const int buttonPin5 = 4;    // button 5
const int buttonPin6 = 5;    // button 6

//Bytes and Shit
byte addrB1 = 0x0B;    // first grizzly address - Spinner 
byte addrB2 = 0x0E;    // second grizzly address - Gate1
byte addrB3 = 0x0F;    // third grizzly address - Gate2


byte currLimitSpinner[3] = {0b00010011, 54, 0};
byte startB[8] = {0b00010011,0x0,0x0,0x0,0x0,100,0x0,0x0};
byte startBSlow[8] = {0b00010011,0x0,0x0,0x0,0x0,30,0x0,0x0};
byte stopB[8] = {0b00010011,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
byte reverseB[8] = {0b00010011,0x0,0x0,0x0,0x0,0x9C,0xFF,0x0};
byte reverseBSlow[8] = {0b00010011,0x0,0x0,0x0,0x0,0xE2,0xFF,0x0};
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
  Serial.begin(9600);
  Serial.println("Started");
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
  pinMode(buttonPin1, INPUT);
  digitalWrite(buttonPin1, HIGH); 
  pinMode(buttonPin2, INPUT);
  digitalWrite(buttonPin2, HIGH); 
  
  pinMode(buttonPin3, INPUT);
  digitalWrite(buttonPin3, HIGH); 
  pinMode(buttonPin4, INPUT);
  digitalWrite(buttonPin4, HIGH); 

  pinMode(buttonPin5, INPUT);
  digitalWrite(buttonPin5, HIGH); 
  pinMode(buttonPin6, INPUT);
  digitalWrite(buttonPin6, HIGH);
  
  Serial.println("Started 2");
}

void loop() {
  Serial.print(digitalRead(buttonPin1));
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
  Serial.println(" ");
  // Test stuff
  if (digitalRead(light) == LOW) {
    digitalWrite(ledPin, LOW);
  }
  else {
    digitalWrite(ledPin, HIGH);
    //Serial.write("this light should be on");
  }
    
  time = millis();
  // Gate 1
  if (digitalRead(buttonPin1) == LOW || digitalRead(buttonPin2) == LOW) {
      digitalWrite(ledPin, LOW);
      if (gate1timer < time) {
        gate1timer = time+10000;
      }
  }
  // Going up
  if ((time < gate1timer-8000) && (gate1timer > 0)) {
    Wire.beginTransmission(addrB2);
    Wire.write(0x1);
    Wire.write(startB,8);
    Wire.endTransmission();
  }
  // Going down
  else if ((time > gate1timer-2000) && (time < gate1timer)) {
    Wire.beginTransmission(addrB2);
    Wire.write(0x1);
    Wire.write(reverseB,8);
    Wire.endTransmission();
  }
  // Stopped
  else {
    Wire.beginTransmission(addrB2);
    Wire.write(0x1);
    Wire.write(stopB,8);
    Wire.endTransmission();
  }

  // Gate 2
  if (digitalRead(buttonPin3) == LOW || digitalRead(buttonPin4) == LOW) {
      digitalWrite(ledPin, LOW);
      if (gate2timer < time) {
        gate2timer = time+10000;
      }
  }
  // Going up
  if ((time < gate2timer-8000) && (gate2timer > 0)) {
    Wire.beginTransmission(addrB3);
    Wire.write(0x1);
    Wire.write(startB,8);
    Wire.endTransmission();
  }
  // Going down
  else if ((time > gate2timer-2000) && (time < gate2timer)) {
    Wire.beginTransmission(addrB3);
    Wire.write(0x1);
    Wire.write(reverseB,8);
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
  if(digitalRead(buttonPin5) == LOW || digitalRead(buttonPin6) == LOW) {
    if (spinnertimer < time) {
      dir = !dir;
      spinnertimer = time+5000;
    }
  }
  if(dir) {
    Serial.println("Forward");
    Wire.beginTransmission(addrB1);
    Wire.write(0x1);
    Wire.write(startBSlow,8);
    Wire.endTransmission();
  }
  else {
    Serial.println("Backward");
    Wire.beginTransmission(addrB1);
    Wire.write(0x1);
    Wire.write(reverseBSlow,8);
    Wire.endTransmission();
  }
}
      // No acceleration
      /*Wire.beginTransmission(addrB2);
      Wire.write(0x90);
      Wire.write(noaccel,2);
      Wire.endTransmission();*/

  /*}else if(time >= gate1timer){
      Wire.beginTransmission(addrB2);
      Wire.write(0x1);
      Wire.write(reverseB,8);
      Wire.endTransmission();
  }
  else{
      Wire.beginTransmission(addrB2);
      Wire.write(0x1);
      Wire.write(stopB,8);
      Wire.endTransmission();
  }*/
  // Lower the 1st gate
  //else if(!gate1up() && gate1down()){
  
  //}
  // Stop the 1st gate
  
  
  // Raise the 2nd gate
  /*if (gate1up() && !gate1down()){
  
  }
  // Lower the 2nd gate
  else if(!gate1up() && gate1down()){
  
  }
  // Stop the 2nd gate
  else{
  
  }*/
  
  
  /*
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {    
    // turn LED on:    
    digitalWrite(ledPin, LOW);  
  }
  else {
    // turn LED off:
    digitalWrite(ledPin, HIGH);
  }
}*/

/*boolean gate1up(){
  int up1 = digitalRead(buttonPin1);
  int up2 = digitalRead(buttonPin2);
  if (up1) //|| up2)
    return true;
  return false;
}
boolean gate1down(){
  int down1 = digitalRead(buttonPin3);
  int down2 = digitalRead(buttonPin4);
  if (down1 || down2)
    return true;
  return false;
}
boolean gate2up(){
  int up1 = digitalRead(buttonPin5);
  int up2 = digitalRead(buttonPin6);
  if (up1 || up2)
    return true;
  return false;
}*/
/*boolean gate2down(){
  int down1 = digitalRead(buttonPin7);
  int down2 = digitalRead(buttonPin8);
  if (down1 || down2)
    return true;
  return false;
}*/
