#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "ButtonEvents.h"
#include "Objects.h"
#include "MemoryAccess.h"

void InitializeFromMemory();

//--------- variables -------

long steps = 100; //step/mm
float DistanceToGo = 100.00; //mm
float tableMax_X = 400; //mm
float tableMax_Y = 120; //mm

void InitializeFromMemory() {
  EEPROM.get(eeMemoryData.eeAddress, eeMemoryData);

  stepperX.setCurrentPosition(eeMemoryData.motorStepsX);
  stepperY.setCurrentPosition(eeMemoryData.motorStepsY);
  MainPage.SetCursorXPos(lcd, eeMemoryData.tablePosX * 2);
  MainPage.SetCursorYPos(lcd, eeMemoryData.tablePosY);

  strBuilder.InitializeDistanceArr( DistanceArr , eeMemoryData.distanceValue);
  strBuilder.InitializeCalibrationArr( CalibrationArrX , eeMemoryData.calibrationValueX);
  strBuilder.InitializeCalibrationArr( CalibrationArrY , eeMemoryData.calibrationValueY);
  DistancePage.PageStringArr[0] = strBuilder.GetDistanceString(DistanceArr);
  CalibrationX.PageStringArr[0] = strBuilder.GetCalibrationString(CalibrationArrX);
  CalibrationY.PageStringArr[0] = strBuilder.GetCalibrationString(CalibrationArrY);
}
void ProgramInitialize() {
  lcd.begin(16, 2);
  displaiyWelcomeScreen();
  pinMode(IR01_PIN, INPUT);
  pinMode(IR02_PIN, INPUT);

  MainPage.PageStringArr[0] = "1 2 3 4";
  MainPage.PageStringArr[1] = "5 6 7 8";
  MainPage.MaxPageWidth = 7;

  //DistancePage.PageStringArr[0] = (String)DistancePage.PageValue;
  DistancePage.PageStringArr[1] = "Stroke Distance";
  DistancePage.MaxPageHeight = 1;
  DistancePage.MaxPageWidth = 7;

  HomingPage.PageStringArr[0] = "";
  HomingPage.PageStringArr[1] = "HOME Position";
  HomingPage.MaxPageHeight = 1;
  HomingPage.IsCursorShown = false;

  //CalibrationX.PageStringArr[0] = (String)CalibrationX.PageValue;;
  CalibrationX.PageStringArr[1] = "Calibrate X-Axis";
  CalibrationX.MaxPageHeight = 1;
  CalibrationX.MaxPageWidth = 4;

  //CalibrationY.PageStringArr[0] = (String)CalibrationY.PageValue;;
  CalibrationY.PageStringArr[1] = "Calibrate Y-Axis";
  CalibrationY.MaxPageHeight = 1;
  CalibrationY.MaxPageWidth = 4;



  stepperX.setMaxSpeed(MotorSpeed);
  stepperX.setAcceleration(MotorAcceleration);
  stepperX.setSpeed(MotorSpeed);


  stepperY.setMaxSpeed(MotorSpeed);
  stepperY.setAcceleration(MotorAcceleration);
  stepperY.setSpeed(MotorSpeed);
  delay(2000);
  NavigateToFirstPage();
}

void ProgramLoop() {

  switch (PageIndex) {
    case 0:
      {
        MainPageBtnEvents();
        break;
      }
    case 1:
      {
        HomingPageBtnEvents();
        break;
      }
    case 2:
      {
        DistancePageBtnEvents();
        break;
      }
    case 3:
      {
        CalibrationXBtnEvents();
        break;
      }
    case 4:
      {
        CalibrationYBtnEvents();
        break;
      }
  }
}


void NavigateToFirstPage() {
  PageIndex = 0;
  if (!(Pages[PageIndex]->IsActive)) {
    Pages[PageIndex]->Navigate(lcd);
    for (int ii = PageIndex ; ii < PagesSize ; ii++) {
      if (ii != PageIndex)
        Pages[ii]->IsActive = false;
    }
  }
}

void NavigateToNextPage(int* pageIndex) {
  if (*pageIndex + 1 < PagesSize) {
    *pageIndex = *pageIndex + 1;
  } else {
    *pageIndex = 0;
  }
  if (!(Pages[*pageIndex]->IsActive)) {
    Pages[*pageIndex]->Navigate(lcd);
  }
  for (int ii = 0 ; ii < PagesSize ; ii++) {
    if (ii != *pageIndex)
      Pages[ii]->IsActive = false;
  }
}


void SetMotorsTargetPosition() {
  stepperX.moveTo(eeMemoryData.distanceValue * eeMemoryData.tablePosX * eeMemoryData.calibrationValueX * MotorDirectionX);
  stepperY.moveTo(eeMemoryData.distanceValue * eeMemoryData.tablePosY * eeMemoryData.calibrationValueY * MotorDirectionY);
}

void StopMotors() {
  stepperY.stop();
  stepperX.stop();
}
void UpdateMotorPositionsToMemory() {
  if (eeMemoryData.motorStepsX != stepperX.currentPosition() ||
      eeMemoryData.motorStepsY != stepperY.currentPosition())
  {
    eeMemoryData.motorStepsX = stepperX.currentPosition();
    eeMemoryData.motorStepsY = stepperY.currentPosition();
    eeMemoryAccess.SaveMotorsPos(eeMemoryData);
  }
}
void ResetEEpromRecords() {
  eeMemoryData.tablePosX = 0;
  eeMemoryData.tablePosY = 0;
  eeMemoryData.motorStepsX = 0;
  eeMemoryData.motorStepsY = 0;
  eeMemoryData.calibrationValueX = 100;
  eeMemoryData.calibrationValueY = 100;
  eeMemoryData.distanceValue = 100.00f;

  EEPROM.put(eeMemoryData.eeAddress, eeMemoryData);
  InitializeFromMemory();
}
void ResetMotorsPos() {
  StopMotors();
  stepperX.setCurrentPosition(0);
  stepperY.setCurrentPosition(0);

  stepperX.setMaxSpeed(MotorSpeed);
  stepperX.setAcceleration(MotorAcceleration);
  stepperX.setSpeed(MotorSpeed);

  stepperY.setMaxSpeed(MotorSpeed);
  stepperY.setAcceleration(MotorAcceleration);
  stepperY.setSpeed(MotorSpeed);

  eeMemoryData.tablePosX = 0;
  eeMemoryData.tablePosY = 0;
  eeMemoryData.motorStepsX = 0;
  eeMemoryData.motorStepsY = 0;
  EEPROM.put(eeMemoryData.eeAddress, eeMemoryData);
  InitializeFromMemory();
}
void displaiyWelcomeScreen() {
  lcd.setCursor(0, 0);
  lcd.print("- Scorpion CNC -");
  lcd.setCursor(0, 1);
  lcd.print("- Syntax Error -");
}
//void ResetMotorsToLimitSwitch(short limSwXPin, short limSwYPin) {
//int ir01 = digitalRead(IR01_PIN);
//    stepperX.setSpeed(-MotorSpeed * MotorDirectionX);
//    stepperY.setSpeed(-MotorSpeed * MotorDirectionY);
//    int ir02 = digitalRead(IR02_PIN);
//    if (!ir02) {
//      stepperY.runSpeed();
//    }
//    else if (!ir01) {
//      stepperX.runSpeed();
//    }
//    else if (ir01 && ir02) {
//      ResetMotorsPos();
//      IsResetToLimitSw = false;
//    }
//}
#endif
