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

// Sensor variables
const byte radioPin = A0; // Radio sensor output
const byte irPin = A1; // IR sensor output
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

// For debugging
int pos = 0;
char inputString[5];
Adafruit_NeoPixel pixels(1, 40, NEO_GRB + NEO_KHZ800); //Enable built in NeoPixel

// freq = false: tune to 61kHz. freq = true: tune to 89kHz.
void tune_radio(bool freq) {
  digitalWrite(2, freq ? HIGH : LOW); // Turn switch on/off to adjust capacitance
  delay(15); // Wait for transient effects to subside
}

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
  // Initialise NeoPixel
  pixels.begin();
  pixels.setBrightness(255);
  
  // Sensors
  pinMode(radioPin, INPUT_PULLDOWN);
  pinMode(2, OUTPUT); // Controls radio frequency tuning
  pinMode(irPin, INPUT_PULLDOWN);
  // Motors
  pinMode(9, OUTPUT); // Left PWM
  pinMode(8, OUTPUT); // Left DIR
  pinMode(6, OUTPUT); // Right PWM
  pinMode(3, OUTPUT); // Right DIR

  // Error out if no WiFi Shield
  if (WiFi.status() == WL_NO_SHIELD){
    Serial.println("WiFi module not present");
    while (true){
      pixels.setPixelColor(0,pixels.Color(255,0,0));
      pixels.show();
      while (true){
        pixels.setBrightness(0);
        pixels.show();
        delay(500);
        pixels.setBrightness(255);
        pixels.show();
        delay(500);
      }
    }
  }
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    pixels.setPixelColor(0,pixels.Color(0,0,255));
    pixels.show();
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  Serial.println("Connected to wifi");
  pixels.setPixelColor(0,pixels.Color(0,255,0));
  pixels.show();
  printWiFiStatus();
  Udp.begin(localPort); 
}

void loop() {
  // Update delta
  delta = millis() - prevTime;
  prevTime = millis();
  
  //Udp inputs
  int packetSize = Udp.parsePacket();
  if(packetSize){
    // Read incoming control packet into buffer.
    int len = Udp.read(packetBuffer, 255);
    if(len > 0){
      packetBuffer[len] = 0;
    }
    Udp.beginPacket(Udp.remoteIP(),Udp.remotePort());
    Udp.write("ACK");
    Udp.endPacket();
    if(packetBuffer[0] == 'Z' && packetBuffer[1] == 'D' && packetBuffer[2] == 'B'){
      if(packetBuffer[3]=='1'){
        CaptureSensorData();
        SendSensorData();
      }
      else{
        sweeping = packetBuffer[8] == '1';
        F = packetBuffer[4] == '1';
        B = packetBuffer[5] == '1';
        L = packetBuffer[6] == '1';
        R = packetBuffer[7] == '1';
      }
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
  analogWrite(9, abs(leftWheelVelocity));
  analogWrite(6, abs(rightWheelVelocity));

  // DIR outputs
  digitalWrite(8, leftWheelVelocity >= 0 ? LOW : HIGH);
  digitalWrite(3, rightWheelVelocity >= 0 ? LOW : HIGH);
}

void printWiFiStatus() {
  // print the SSID of the network
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

  // print WiFi shield's IP address
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

  // print the received signal strength
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

double frequencyDetector(byte pin, int lowerThreshold, int upperThreshold,int sampleTime = 50){
    long t = millis();
    bool rising = false;
    int i = 0;
    double freq = 0; 
    long signalTime;
    bool trig = true;
    do{
        int sample = analogRead(pin);
        if(sample > upperThreshold && rising){
            if(trig){
                trig = false;
            }
            else{
                long d = micros()-signalTime + 43.7; //Delay compensation calculated at 500Hz reference frequency 
                i++;
                freq += 1.0/(d) * 1000000;
            }
            signalTime = micros();
            rising = false;
        }
        if(sample < lowerThreshold && !rising){
            rising = true;
        }
    }while(millis() - t < sampleTime);
    return freq / i;
}


void CaptureSensorData(){
    // Radio sensor
    tune_radio(false); // Tune to 61kHz
    SensorData["radio61k"] = frequencyDetector(radioPin, 10, 40, 100);
    tune_radio(true); // Tune to 89kHz
    SensorData["radio89k"] = frequencyDetector(radioPin, 10, 40, 100);

    // IR sensor
    SensorData["infrared"] = 0; // frequencyDetector(infraredPin,10,50,200);
    
    //SensorData["accoustic"] = 0; //frequencyDetector(accousticPin,10,100,30);
}

void SendSensorData(){
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    serializeJson(SensorData,Udp);
    Udp.endPacket();
}
