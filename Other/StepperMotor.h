#ifndef STEPPER_MOTOR_H_
#define STEPPER_MOTOR_H_

#include <Arduino.h>
#include "Objects.h"
class StepperMotor {
  public:
    short ClockPin;
    short DirPin;
    short EnablePin;

    float DeltaTime = 3000 ;
    //float MaxDeltaTime = 6000;
    //float MinDeltaTime = 500; //us
    float ClockTimeDelta = 1000;

    //float Acceleration = 200;
    //float Speed = 200;
    long Steps = 0;
    long Calibration = 100;
    //bool IsEnableActiveLOW = false;
    //bool InvertDireciton = false;
    bool IsRunning = false;
    float Distance = 10000; //mm
    float LastClockTime;

    StepperMotor(short clockPin, short dirPin, short enablePin) {
      ClockPin = clockPin;
      DirPin = dirPin;
      EnablePin = enablePin;
      Initialize();
    }

    void Initialize() {
      pinMode(DirPin , OUTPUT);
      pinMode(ClockPin , OUTPUT);
      pinMode(EnablePin , OUTPUT);
      digitalWrite(EnablePin , LOW);
    }


    bool run() {
      {
        Serial.begin(9600);
        IsRunning = true;
        if (IsRunning) {
          SetDirectionPin();
          LastClockTime = micros();
          for (long stp = 0 ; stp < Steps; stp ++) {
            while ((micros() - LastClockTime) < DeltaTime) {
              IsCanceled();
              if (!IsRunning) {
                break;
              }
            }
            IsCanceled();
            if (!IsRunning) {
              break;
            }
            digitalWrite(ClockPin, HIGH);
            delay(5);
            digitalWrite(ClockPin, LOW);
            LastClockTime = micros();
          }
          IsRunning = false;
        }
      }

    }
    void SetDirectionPin() {
      if (Steps < 0)
      {
        digitalWrite(DirPin, HIGH);
        Steps *= -1;
      }
      else {
        digitalWrite(DirPin, LOW);
      }
    }

    void IsCanceled() {
      IsRunning = ! cancelButton.GetState();
    }
    void SetDistance(float distance) {
      Distance = distance;
      Steps = distance * Calibration;
    }
    void SetCalibration(long calibration) {
      Calibration = calibration;
    }
};
#endif
