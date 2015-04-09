#include "Wire.h"

// Gate 1 Up
const int buttonPin1 = 2;    // button 1
const int buttonPin2 = 2;    // button 2
// Gate 1 Down
const int buttonPin3 = 4;    // button 3
const int buttonPin4 = 4;    // button 4

// Gate 2 Up
const int buttonPin5 = 5;    // button 5
const int buttonPin6 = 6;    // button 6
// Gate 2 Down
const int buttonPin7 = 7;    // button 7
const int buttonPin8 = 8;    // button 8

//Bytes and Shit
byte addrB1 = 0x0B;    // first grizzly address - Spinner 
byte addrB2 = 0x0E;    // second grizzly address - Gate1
byte addrB3 = 0x0F;    // third grizzly address - Gate2


byte startB[8] = {0b00010011,0x0,0x0,0x0,0x0,100,0x0,0x0};
byte stopB[8] = {0b00010011,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
byte reverseB[8] = {0b00010011,0x0,0x0,0x0,0x0,0x9C,0xFF,0x0};
byte noaccel[2] = {0b00010011, 20};
byte setToutB[2] = {0x0, 0x08};

const int ledPin =  13;      // the number of the LED pin
int buttonState = 0;         // variable for reading the pushbutton status
#define ON LOW
#define OFF HIGH

unsigned long time;
int gate1timer;    // Lower after 10 seconds


void setup() {
  // initialize the LED pin as an output:
  //pinMode(ledPin, OUTPUT);      
  
  // Initialize I2C
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(addrB1);
  Wire.write(0x80);
  Wire.write(setToutB,2);
  Wire.endTransmission();
  
  time = millis();
  gate1timer=time;

  // Initialize buttons
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

  pinMode(buttonPin7, INPUT);
  digitalWrite(buttonPin7, HIGH); 
  pinMode(buttonPin8, INPUT);
  digitalWrite(buttonPin8, HIGH); 
}

void loop(){
  // Raise the 1st gate
  time = millis();
  if (gate1up()){ //&& !gate1down()){
      if (gate1timer < time) {
        gate1timer = time+5000;
      }
      Wire.beginTransmission(addrB2);
      Wire.write(0x1);
      Wire.write(startB,8);
      Wire.endTransmission();
      
      // No acceleration
      Wire.beginTransmission(addrB2);
      Wire.write(0x90);
      Wire.write(noaccel,2);
      Wire.endTransmission();

  }else if(time >= gate1timer){
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
  }
  // Lower the 1st gate
  //else if(!gate1up() && gate1down()){
  
  //}
  // Stop the 1st gate
  
  
  // Raise the 2nd gate
  if (gate1up() && !gate1down()){
  
  }
  // Lower the 2nd gate
  else if(!gate1up() && gate1down()){
  
  }
  // Stop the 2nd gate
  else{
  
  }
  
  
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
  }*/
}

boolean gate1up(){
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
}
boolean gate2down(){
  int down1 = digitalRead(buttonPin7);
  int down2 = digitalRead(buttonPin8);
  if (down1 || down2)
    return true;
  return false;
}
