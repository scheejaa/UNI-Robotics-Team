#ifndef PS5CONTROLLER_H
#define PS5CONTROLLER_H

#include <ps5Controller.h>

class PS5Controller {
public:
  PS5Controller();
  void begin(const char *macAddress);
  void attachCallbacks();
  void notify();
  bool isConnected();

private:
  void onConnect();
  void onDisConnect();
  void controlMotors();
  unsigned long lastTimeStamp;
};

#endif // PS5CONTROLLER_H
