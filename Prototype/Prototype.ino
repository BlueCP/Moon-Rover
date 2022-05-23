// Steering/movement constants
const float TURNING_FACTOR = 0.5; // Number between 0 and 1 to control maximum sharpness of turns
const float RESPONSE_SPEED = 0.1; // Controls how quickly a wheel reaches the target velocity
const float SWEEP_PERIOD = 3000; // Period of sweeping cycle in ms
const int SWEEP_WIDTH = 0x20; // How wide the sweep range should be

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
  
  // Motors
  pinMode(2, OUTPUT); // Left PWM
  pinMode(3, OUTPUT); // Left DIR
  pinMode(4, OUTPUT); // Left Vin
  pinMode(5, OUTPUT); // Right PWM
  pinMode(6, OUTPUT); // Right DIR
  pinMode(7, OUTPUT); // Right Vin
}

void loop() {
  // Update delta
  delta = millis() - prevTime;
  prevTime = millis();

  // Input detection - temporary
  if (Serial.available() > 0) {
    char inputByte = Serial.read();
    if (inputByte == '1' || inputByte == '0') {
      inputString[pos] = inputByte;
      pos++;
    }
    if (pos == 5) {
      pos = 0;
      F = inputString[0] == '1' ? true : false;
      B = inputString[1] == '1' ? true : false;
      L = inputString[2] == '1' ? true : false;
      R = inputString[3] == '1' ? true : false;
      sweeping = inputString[4] == '1' ? true : false;
    } else {
      return;
    }
  }

  if (sweeping) { // Note that sweeping overrides all other inputs
    sweep();
  } else {
    sweepTime = 0;
    left_velocity();
    right_velocity();
  }  

  // Debug
  Serial.print(leftWheelVelocity);
  Serial.print("\t");
  Serial.print(rightWheelVelocity);

  // PWM outputs
  if (leftWheelVelocity == 0) {
    analogWrite(2, 0);
  } else {
    analogWrite(2, 0xff);
  }
  if (rightWheelVelocity == 0) {
    analogWrite(5, 0);
  } else {
    analogWrite(5, 0xff);
  }

  // DIR outputs
  if (leftWheelVelocity > 0) {
    analogWrite(3, 0);
  } else {
    analogWrite(3, 0xff);
  }
  if (rightWheelVelocity > 0) {
    analogWrite(6, 0);
  } else {
    analogWrite(6, 0xff);
  }

  // Vin outputs
  analogWrite(4, abs(leftWheelVelocity));
  analogWrite(7, abs(rightWheelVelocity));

  Serial.println(""); // Terminate serial printing
}
