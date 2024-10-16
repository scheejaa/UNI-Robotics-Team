#include <ESP32Servo.h>
#include <ps5Controller.h>

Servo servoLeft;
Servo servoRight;

#define SERVO_LEFT_PIN 22
#define SERVO_RIGHT_PIN 23

unsigned long lastTimeStamp = 0;

void setup() {
  servoLeft.attach(SERVO_LEFT_PIN);
  servoRight.attach(SERVO_RIGHT_PIN);

  Serial.begin(115200);
  ps5.attach(notify);
  ps5.attachOnConnect(onConnect);
  ps5.attachOnDisconnect(onDisConnect);
  ps5.begin("24:a6:fa:ca:d0:75"); // your PS5 controller MAC address

  while (!ps5.isConnected()) {
    Serial.println("PS5 controller not found");
    delay(300);
  }

  Serial.println("Ready.");
}

void notify() {
  char messageString[200];
  sprintf(messageString, "%4d,%4d,%4d,%4d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d",
          ps5.LStickX(), ps5.LStickY(), ps5.RStickX(), ps5.RStickY(),
          ps5.Left(), ps5.Down(), ps5.Right(), ps5.Up(),
          ps5.Square(), ps5.Cross(), ps5.Circle(), ps5.Triangle(),
          ps5.L1(), ps5.R1(), ps5.L2(), ps5.R2(),
          ps5.Share(), ps5.Options(), ps5.PSButton(), ps5.Touchpad(),
          ps5.Charging(), ps5.Audio(), ps5.Mic(), ps5.Battery());

  if (millis() - lastTimeStamp > 50) {
    Serial.println(messageString);
    lastTimeStamp = millis();
  }

  controlMotors();
}

void controlMotors() {
  int leftStickY = ps5.LStickY(); // Read left stick Y-axis
  int rightStickY = ps5.RStickY(); // Read right stick Y-axis

  // Map the stick values to the servo range
  int leftServoValue = map(leftStickY, -128, 127, 2500, 500);
  int rightServoValue = map(rightStickY, -128, 127, 500, 2500);

  // Control the servos based on joystick positions
  servoLeft.writeMicroseconds(leftServoValue);
  servoRight.writeMicroseconds(rightServoValue);
}

void onConnect() {
  Serial.println("Connected!");
}

void onDisConnect() {
  Serial.println("Disconnected!");
}

void loop() {
  // Nothing to do here
}