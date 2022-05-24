#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
//#include "WiFi_Secret.h"
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>

// Wireless variables
int status = WL_IDLE_STATUS;
char ssid[] = "Nik";
char pass[] = "xyz55555";
unsigned int localPort = 2390;
char packetBuffer[255];
WiFiUDP Udp;

// Signal detection variables
long signalTime; //Stores time since last pulse
double freq; //calculated from signalTime
const byte interruptPin = 9; // Pin that will detect the pulses
StaticJsonDocument<128> SensorData; // Seensor data to be transmitted 128 bytes large

// Steering/movement constants
const float TURNING_FACTOR = 0.5; // Number between 0 and 1 to control maximum sharpness of turns
const float RESPONSE_SPEED = 0.1; // Controls how quickly a wheel reaches the target velocity
const float SWEEP_PERIOD = 3000; // Period of sweeping cycle in ms
const int SWEEP_WIDTH = 0xb0; // How wide the sweep range should be

// General useful values
unsigned long prevTime = 0;
int delta = 0; // Time since last loop() call

// Wheel state
bool F, B, L, R = false;
int leftWheelVelocity = 0;
int rightWheelVelocity = 0;
int sweepTime = 0;
bool sweeping = false;
bool Controls[8]; // Control layout [T,R,B,L,F,S,X,X]
/* 
  X - Unused
  S - Sweep
  T - Test sample
  F, L, B, R - Direction
*/


// For debugging
int pos = 0;
char inputString[5];

int exponential_steering(int current, int target) {
  int difference = abs(current - target);
  if (current < target) {
    current += difference * RESPONSE_SPEED;
    if (current > target) {
      current = target;
    }
  } else if (current > target) {
    current -= difference * RESPONSE_SPEED;
    if (current < target) {
      current = target;
    }
  }
  return current;
}

void sweep() {
  float val = SWEEP_WIDTH * cos((2*PI) / SWEEP_PERIOD * sweepTime);
  leftWheelVelocity = -val;
  rightWheelVelocity = val;
  sweepTime += delta;
}

// Calculate left wheel velocity
void left_velocity() {
  int leftTargetSpeed;
  if (F == B && L == R) {
    leftTargetSpeed = 0;
  } else if (F != B && L && !R) {
    leftTargetSpeed = TURNING_FACTOR * 0xff;
  } else {
    leftTargetSpeed = 0xff;
  }
  if ((!F && B) || (F == B && L && !R)) {
    leftWheelVelocity = exponential_steering(leftWheelVelocity, -leftTargetSpeed);
  } else {
    leftWheelVelocity = exponential_steering(leftWheelVelocity, leftTargetSpeed);
  }
}

// Calculate right wheel velocity
void right_velocity() {
  int rightTargetSpeed;
  if (F == B && L == R) {
    rightTargetSpeed = 0;
  } else if (F != B && !L && R) {
    rightTargetSpeed = TURNING_FACTOR * 0xff;
  } else {
    rightTargetSpeed = 0xff;
  }
  if ((!F && B) || (F == B && !L && R)) {
    rightWheelVelocity = exponential_steering(rightWheelVelocity, -rightTargetSpeed);
  } else {
    rightWheelVelocity = exponential_steering(rightWheelVelocity, rightTargetSpeed);
  }
}

void setup() {
  // Initialise serial output
  Serial.begin(9600);

  // Sensor
  pinMode(interruptPin, INPUT_PULLDOWN);
  
  // Motors
  pinMode(5, OUTPUT); // Left PWM
  pinMode(4, OUTPUT); // Left DIR
  pinMode(3, OUTPUT); // Right PWM
  pinMode(2, OUTPUT); // Right DIR

  // Error out if no WiFi Shield
  if (WiFi.status() == WL_NO_SHIELD){
    Serial.println("WiFi shield not present");;
    while (true){}
  }
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();
  Udp.begin(localPort); 
}

void loop() {
  // Update delta
  delta = millis() - prevTime;
  prevTime = millis();

  // Input detection - temporary
  // if (Serial.available() > 0) {
  //   char inputByte = Serial.read();
  //   if (inputByte == '1' || inputByte == '0') {
  //     inputString[pos] = inputByte;
  //     pos++;
  //   }
  //   if (pos == 5) {
  //     pos = 0;
  //     F = inputString[0] == '1' ? true : false;
  //     B = inputString[1] == '1' ? true : false;
  //     L = inputString[2] == '1' ? true : false;
  //     R = inputString[3] == '1' ? true : false;
  //     sweeping = inputString[4] == '1' ? true : false;
  //   } else {
  //     return;
  //   }
  // }

  // Udp inputs
  int packetSize = Udp.parsePacket();
  if(packetSize){
    // Read incoming control packet into buffer.
    int len = Udp.read(packetBuffer, 255);
    if(len > 0){
      packetBuffer[len] = 0;
    }
    // Convert single byte into an array of 8 bools
    for (int i=0; i < 8; ++i){
        Controls[i] = (packetBuffer[0] & (1<<i)) != 0;
    }
    if(Controls[0]){
      CaptureSensorData();
      SendSensorData();
    }
    else{
      sweeping = Controls[5];
      F = Controls[4];
      L = Controls[3];
      B = Controls[2];
      R = Controls[1];
    }
  }

  if (sweeping) { // Note that sweeping overrides all other inputs
    sweep();
  } else {
    sweepTime = 0;
    left_velocity();
    right_velocity();
  }  

  // PWM outputs
  analogWrite(5, abs(leftWheelVelocity));
  analogWrite(3, abs(rightWheelVelocity));

  // DIR outputs
  digitalWrite(4, leftWheelVelocity >= 0 ? LOW : HIGH);
  digitalWrite(2, rightWheelVelocity >= 0 ? LOW : HIGH);
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

  // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

void CaptureSensorData(){
    attachInterrupt(digitalPinToInterrupt(interruptPin),count,RISING);
    delay(100);
    detachInterrupt(digitalPinToInterrupt(interruptPin));
    SensorData["radio"] = freq;
    // attachInterrupt(digitalPinToInterrupt(infraredPin),count,RISING);
    // delay(100);
    // detachInterrupt(digitalPinToInterrupt(infraredPin));
    SensorData["infrared"] = freq;
    // attachInterrupt(digitalPinToInterrupt(accousticPin),count,RISING);
    // delay(100);
    // detachInterrupt(digitalPinToInterrupt(accousticPin));
    SensorData["accoustic"] = freq;
}

void SendSensorData(){

    // Debug
    //serializeJson(SensorData,Serial);
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    serializeJson(SensorData,Udp);
    Udp.endPacket();
}

void count(){
    long d = micros()-signalTime;
    freq = 1.0/(d) * 1000000;
    signalTime = micros();
}
