#include "PS5Controller.h"
#include <ESP32Servo.h>

Servo servoFrontLeft;
Servo servoFrontRight;
Servo servoRearLeft;
Servo servoRearRight;

PS5Controller::PS5Controller() : lastTimeStamp(0) {}

void PS5Controller::begin(const char *macAddress) {
  servoFrontLeft.attach(19);
  servoFrontRight.attach(20);
  servoRearLeft.attach(21);
  servoRearRight.attach(22);

  Serial.begin(115200);
  ps5.attach([this]() { this->notify(); });
  ps5.attachOnConnect([this]() { this->onConnect(); });
  ps5.attachOnDisconnect([this]() { this->onDisConnect(); });
  ps5.begin(macAddress);

  while (!ps5.isConnected()) {
    Serial.println("PS5 controller not found");
    delay(300);
  }
  Serial.println("Ready.");
}

void PS5Controller::attachCallbacks() {
  ps5.attach([this]() { this->notify(); });
  ps5.attachOnConnect([this]() { this->onConnect(); });
  ps5.attachOnDisconnect([this]() { this->onDisConnect(); });
}

void PS5Controller::notify() {
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

void PS5Controller::controlMotors() {
  int leftStickY = ps5.LStickY();
  int leftStickX = ps5.LStickX();
  int rightStickX = ps5.RStickX();

  int frontLeftServoValue = map(leftStickY + leftStickX + rightStickX, -384, 384, 500, 2500);
  int frontRightServoValue = map(leftStickY - leftStickX - rightStickX, -384, 384, 500, 2500);
  int rearLeftServoValue = map(leftStickY - leftStickX + rightStickX, -384, 384, 500, 2500);
  int rearRightServoValue = map(leftStickY + leftStickX - rightStickX, -384, 384, 500, 2500);

  servoFrontLeft.writeMicroseconds(frontLeftServoValue);
  servoFrontRight.writeMicroseconds(frontRightServoValue);
  servoRearLeft.writeMicroseconds(rearLeftServoValue);
  servoRearRight.writeMicroseconds(rearRightServoValue);
}

void PS5Controller::onConnect() {
  Serial.println("Connected!");
}

void PS5Controller::onDisConnect() {
  Serial.println("Disconnected!");
}

bool PS5Controller::isConnected() {
  return ps5.isConnected();
}
