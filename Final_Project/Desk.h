/*
  Desk.h - Library for controlling motorized standing desk.
  Created by Simon Ng, May 29, 2020. Last edit June 5, 2020
*/

#ifndef Desk_h // protects against bugs if user #includes this library twice
#define Desk_h // protects against bugs if user #includes this library twice

#include "Arduino.h"
#include <Servo.h>
#include<Wire.h>

class Desk
{
  public: // need to access these function externally from main script
    Desk(byte PIN_servo1, byte PIN_servo2, byte PIN_servo3, byte PIN_servo4, byte PIN_joystick);
    void initialize();
    void moveArm(int servo, String dir, int distance, boolean useDistance);
    void movePlatform(String dir, int distance);
    void stopPlatform();
    void levelPlatform();
    void cofigureAccelerometer();
    void getAccel(float * ptrToArray);
    void printAccel();
    int getDeltaHeight();
    int getContDeltaHeight();
    void configureThermistor();
    void printThermistorDiagnostics();
    void printByte(byte myByte);
    float getAccelIn(char accelDir);
    void movePlatformGradual(String dir, int velocity);
    void Troubleshoot();

  private: // create servo objects and accelerometer variables as internal, private variables
    byte _PIN_servo1;
    byte _PIN_servo2;
    byte _PIN_servo3;
    byte _PIN_servo4;
    byte _PIN_joystick;
    Servo servo1;  // create servo object to control a servo
    Servo servo2;  // create servo object to control a servo
    Servo servo3;  // create servo object to control a servo
    Servo servo4;  // create servo object to control a servo
    float _accel[3];
    int _joyPosOld = 0;
};

#endif // protects against bugs if user #includes this library twice
