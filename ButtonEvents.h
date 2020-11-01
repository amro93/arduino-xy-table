#ifndef BUTTON_EVENTS_H_
#define BUTTON_EVENTS_H_

#include "Objects.h"
#include "MemoryAccess.h"

//---------func declaration ----------
void MainPageBtnEvents();
void DistancePageXBtnEvents();
void DistancePageYBtnEvents();
void HomingPageBtnEvents();
void CalibrationXBtnEvents();
void CalibrationYBtnEvents();
void NavigateToFirstPage();
void NavigateToNextPage(int* pageIndex);
void UpdateMotorPositionsToMemory();
void ResetEEpromRecords();
void ResetMotorsPos();
//void ResetMotorsToLimitSwitch(short limSwXPin, short limSwYPin);
void displaiyWelcomeScreen();

void SetMotorsTargetPosition();
void StopMotors();

//------ variables --------
bool isTablePositionChanged;
int PageIndex = 0;
//int TablePositionIndex = 0;

//int eeMemoryData.tablePosX = 0;
//int eeMemoryData.tablePosY = 0;

short CalibrationArrX[4] = {0, 1, 0, 0} ;
short CalibrationArrY[4] = {0, 1, 0, 0} ;
short DistanceArrX[6] = {0, 1, 0, 0, 0, 0} ;
short DistanceArrY[6] = {0, 1, 0, 0, 0, 0} ;

int MotorSpeed = 2000;
int MotorAcceleration = 6000;
short MotorDirectionX = -1;
short MotorDirectionY = 1;

int cnt = 0;
short okButtonLevel = 0;
short cancelButtonLevel = 0;

//  eeMemoryData.tablePosX = 0;
//  eeMemoryData.tablePosY = 0;
//  eeMemoryData.motorStepsX = 0;
//  eeMemoryData.motorStepsY = 0;
//  eeMemoryData.calibrationValueX = 100;
//  eeMemoryData.calibrationValueY = 100;
//  eeMemoryData.distanceValueX = 100.00f;
//  eeMemoryData.distanceValueY = 100.00f;
//  eeMemoryAccess.SaveData(eeMemoryData.eeAddress, eeMemoryData);

//------- MainPage ------
void MainPageBtnEvents() {
  stepperX.run();
  stepperY.run();

  okButtonLevel = okButton.GetHoldLevel();
  if (okButtonLevel == 1) {
    NavigateToNextPage(&PageIndex);
  }
  else if (okButtonLevel == 2) {
    isTablePositionChanged = true;
  }
  else if (cancelButton.IsPressed()) {
    StopMotors();
    NavigateToFirstPage();
  }
  else if (upButton.IsPressed()) {
    if (eeMemoryData.tablePosY  > 0) {
      eeMemoryData.tablePosY--;
      MainPage.SetCursorYPos(lcd, eeMemoryData.tablePosY);
    }
  }
  else if (downButton.IsPressed()) {
    if (eeMemoryData.tablePosY  < 1) {
      eeMemoryData.tablePosY++;
      MainPage.SetCursorYPos(lcd, eeMemoryData.tablePosY);
    }
  }
  else if (rightButton.IsPressed()) {
    if (eeMemoryData.tablePosX   < 3 ) {
      eeMemoryData.tablePosX++;
      MainPage.SetCursorXPos(lcd, eeMemoryData.tablePosX * 2);
    }
  }
  else if (leftButton.IsPressed()) {
    if (eeMemoryData.tablePosX  > 0) {
      eeMemoryData.tablePosX--;
      MainPage.SetCursorXPos(lcd, eeMemoryData.tablePosX * 2);
    }
  }
  if (isTablePositionChanged) {
    SetMotorsTargetPosition();
    eeMemoryAccess.SaveTablePos(eeMemoryData);
    isTablePositionChanged = false;
  }
  if (!stepperX.run() && !stepperY.run()) {
    UpdateMotorPositionsToMemory();
  }
}
//bool gg1 = 0 ;
//bool gg2 = 0 ;
//------HomingPage-------
//bool IsResetToLimitSw = false;
//bool StateFlag = false;
void HomingPageBtnEvents() {
  okButtonLevel = okButton.GetHoldLevel();
  cancelButtonLevel = cancelButton.GetHoldLevel();
  if (okButtonLevel == 1) {
    NavigateToNextPage(&PageIndex);
  }
  else if (okButtonLevel == 2) {
    ResetMotorsPos();
    //IsResetToLimitSw = true;
  }
  else if (cancelButtonLevel == 1) {
    //    IsResetToLimitSw = false;
    NavigateToFirstPage();
  }
  else if (cancelButtonLevel == 2) {
    ResetEEpromRecords();
  }
  else if (upButton.GetState()) {
    stepperY.setSpeed(MotorSpeed * MotorDirectionY);
    stepperY.runSpeed();
  }
  else if (downButton.GetState()) {
    stepperY.setSpeed(-MotorSpeed * MotorDirectionY);
    if (!digitalRead(IR02_PIN)) {
      stepperY.runSpeed();
    }
  }
  else if (rightButton.GetState()) {
    stepperX.setSpeed(MotorSpeed * MotorDirectionX);
    stepperX.runSpeed();
  }
  else if (leftButton.GetState()) {
    stepperX.setSpeed(-MotorSpeed * MotorDirectionX);
    if (!digitalRead(IR01_PIN)) {
      stepperX.runSpeed();
    }
  }
  //  if (IsResetToLimitSw) {
  //    ResetMotorsToLimitSwitch(IR01_PIN, IR02_PIN);
  //  }
}

//------DistancePageY-----
void DistancePageYBtnEvents() {
  short cursorXPos ;
  okButtonLevel = okButton.GetHoldLevel();
  if (okButtonLevel == 1) {
    strBuilder.InitializeDistanceArr(DistanceArrY, eeMemoryData.distanceValueY);
    DistancePageY.PageStringArr[0] = strBuilder.GetDistanceString(DistanceArrY);
    NavigateToNextPage(&PageIndex);
  }
  else if (okButtonLevel == 2) { //<<<<<<<<<<<<<<<<< OK
    eeMemoryData.distanceValueY = strBuilder.CalcDistance(DistanceArrY);
    eeMemoryAccess.SaveData(eeMemoryData);
  }
  else if (cancelButton.IsPressed()) { //<<<<<<<<<<<<<<<<< CANCEL
    StopMotors();
    strBuilder.InitializeDistanceArr(DistanceArrY, eeMemoryData.distanceValueY);
    DistancePageY.PageStringArr[0] = strBuilder.GetDistanceString(DistanceArrY);
    NavigateToFirstPage();
  }
  else if (upButton.IsPressed()) { //<<<<<<<<<<<<<<<<< UP

    if (DistancePageY.GetCursorXPos() < 4) {
      cursorXPos = DistancePageY.GetCursorXPos();
    }
    else if (DistancePageY.GetCursorXPos() > 4) {
      cursorXPos = DistancePageY.GetCursorXPos() - 1;
    }

    if (DistanceArrY[cursorXPos] < 9) {
      DistanceArrY[cursorXPos] += 1;
    }
    else {
      DistanceArrY[cursorXPos] = 0;
    }
    DistancePageY.PageStringArr[0] = strBuilder.GetDistanceString(DistanceArrY);
    DistancePageY.UpdateScreen(lcd);
    DistancePageY.PageDistanceValue = strBuilder.CalcDistance(DistanceArrY);
  }

  else if (downButton.IsPressed()) { //<<<<<<<<<<<<<<<<< DOWN
    if (DistancePageY.GetCursorXPos() < 4) {
      cursorXPos = DistancePageY.GetCursorXPos();
    }
    else if (DistancePageY.GetCursorXPos() > 4) {
      cursorXPos = DistancePageY.GetCursorXPos() - 1;
    }

    if (DistanceArrY[cursorXPos] > 0) {
      DistanceArrY[cursorXPos] -= 1;
    }
    else {
      DistanceArrY[cursorXPos] = 9;
    }
    DistancePageY.PageStringArr[0] = strBuilder.GetDistanceString(DistanceArrY);
    DistancePageY.UpdateScreen(lcd);
    DistancePageY.PageDistanceValue = strBuilder.CalcDistance(DistanceArrY);
  }


  else if (rightButton.IsPressed()) { //<<<<<<<<<<<<<<<<< RIGHT
    DistancePageY.MoveCursorX(lcd, 1);
    if (DistancePageY.GetCursorXPos() == 4) {
      DistancePageY.MoveCursorX(lcd, 1);
    }
  }
  else if (leftButton.IsPressed()) { //<<<<<<<<<<<<<<<<< LEFT
    DistancePageY.MoveCursorX(lcd, -1);
    if (DistancePageY.GetCursorXPos() == 4) {
      DistancePageY.MoveCursorX(lcd, -1);
    }
  }
}


//------DistancePageX-----
void DistancePageXBtnEvents() {
  short cursorXPos ;
  okButtonLevel = okButton.GetHoldLevel();
  if (okButtonLevel == 1) {
    strBuilder.InitializeDistanceArr(DistanceArrX, eeMemoryData.distanceValueX);
    DistancePageX.PageStringArr[0] = strBuilder.GetDistanceString(DistanceArrX);
    NavigateToNextPage(&PageIndex);
  }
  else if (okButtonLevel == 2) { //<<<<<<<<<<<<<<<<< OK
    eeMemoryData.distanceValueX = strBuilder.CalcDistance(DistanceArrX);
    eeMemoryAccess.SaveData(eeMemoryData);
  }
  else if (cancelButton.IsPressed()) { //<<<<<<<<<<<<<<<<< CANCEL
    StopMotors();
    strBuilder.InitializeDistanceArr(DistanceArrX, eeMemoryData.distanceValueX);
    DistancePageX.PageStringArr[0] = strBuilder.GetDistanceString(DistanceArrX);
    NavigateToFirstPage();
  }
  else if (upButton.IsPressed()) { //<<<<<<<<<<<<<<<<< UP
    if (DistancePageX.GetCursorXPos() < 4) {
      cursorXPos = DistancePageX.GetCursorXPos();
    }
    else if (DistancePageX.GetCursorXPos() > 4) {
      cursorXPos = DistancePageX.GetCursorXPos() - 1;
    }
    if (DistanceArrX[cursorXPos] < 9) {
      DistanceArrX[cursorXPos] += 1;
    }
    else {
      DistanceArrX[cursorXPos] = 0;
    }
    DistancePageX.PageStringArr[0] = strBuilder.GetDistanceString(DistanceArrX);
    DistancePageX.UpdateScreen(lcd);
    DistancePageX.PageDistanceValue = strBuilder.CalcDistance(DistanceArrX);
  }

  else if (downButton.IsPressed()) { //<<<<<<<<<<<<<<<<< DOWN
    if (DistancePageX.GetCursorXPos() < 4) {
      cursorXPos = DistancePageX.GetCursorXPos();
    }
    else if (DistancePageX.GetCursorXPos() > 4) {
      cursorXPos = DistancePageX.GetCursorXPos() - 1;
    }

    if (DistanceArrX[cursorXPos] > 0) {
      DistanceArrX[cursorXPos] -= 1;
    }
    else {
      DistanceArrX[cursorXPos] = 9;
    }
    DistancePageX.PageStringArr[0] = strBuilder.GetDistanceString(DistanceArrX);
    DistancePageX.UpdateScreen(lcd);
    DistancePageX.PageDistanceValue = strBuilder.CalcDistance(DistanceArrX);
  }


  else if (rightButton.IsPressed()) { //<<<<<<<<<<<<<<<<< RIGHT
    DistancePageX.MoveCursorX(lcd, 1);
    if (DistancePageX.GetCursorXPos() == 4) {
      DistancePageX.MoveCursorX(lcd, 1);
    }
  }
  else if (leftButton.IsPressed()) { //<<<<<<<<<<<<<<<<< LEFT
    DistancePageX.MoveCursorX(lcd, -1);
    if (DistancePageX.GetCursorXPos() == 4) {
      DistancePageX.MoveCursorX(lcd, -1);
    }
  }
}


//------CalibrationX-------

void CalibrationXBtnEvents() {
  short cursorXPos ;
  okButtonLevel = okButton.GetHoldLevel();
  if (okButtonLevel == 1) {
    strBuilder.InitializeCalibrationArr(CalibrationArrX, eeMemoryData.calibrationValueX);
    CalibrationX.PageStringArr[0] = strBuilder.GetCalibrationString(CalibrationArrX);
    NavigateToNextPage(&PageIndex);
  }
  else if (okButtonLevel == 2) { //<<<<<<<<<<<<<<<<< OK
    eeMemoryData.calibrationValueX = strBuilder.CalcCalibration(CalibrationArrX);
    eeMemoryAccess.SaveData(eeMemoryData);
  }
  else if (cancelButton.IsPressed()) { //<<<<<<<<<<<<<<<<< CANCEL
    StopMotors();
    strBuilder.InitializeCalibrationArr(CalibrationArrX, eeMemoryData.calibrationValueX);
    CalibrationX.PageStringArr[0] = strBuilder.GetCalibrationString(CalibrationArrX);
    NavigateToFirstPage();
  }
  else if (upButton.IsPressed()) { //<<<<<<<<<<<<<<<<< UP
    cursorXPos = CalibrationX.GetCursorXPos();
    if (CalibrationArrX[cursorXPos] < 9) {
      CalibrationArrX[cursorXPos] += 1;
    }
    else {
      CalibrationArrX[cursorXPos] = 0;
    }
    CalibrationX.PageStringArr[0] = strBuilder.GetCalibrationString(CalibrationArrX);
    CalibrationX.UpdateScreen(lcd);
    CalibrationX.PageCalibrationValue = strBuilder.CalcCalibration(CalibrationArrX);
  }
  else if (downButton.IsPressed()) { //<<<<<<<<<<<<<<<<< DOWN
    cursorXPos = CalibrationX.GetCursorXPos();
    if (CalibrationArrX[cursorXPos] > 0) {
      CalibrationArrX[cursorXPos] -= 1;
    }
    else
      CalibrationArrX[cursorXPos] = 9;
    CalibrationX.PageStringArr[0] = strBuilder.GetCalibrationString(CalibrationArrX);
    CalibrationX.UpdateScreen(lcd);
    CalibrationX.PageCalibrationValue = strBuilder.CalcCalibration(CalibrationArrX);
  }
  else if (rightButton.IsPressed()) { //<<<<<<<<<<<<<<<<< RIGHT
    CalibrationX.MoveCursorX(lcd, 1);
  }
  else if (leftButton.IsPressed()) { //<<<<<<<<<<<<<<<<< LEFT
    CalibrationX.MoveCursorX(lcd, -1);
  }
}
//------CalibrationY-------

void CalibrationYBtnEvents() {
  short cursorXPos ;
  okButtonLevel = okButton.GetHoldLevel();
  if (okButtonLevel == 1) {
    strBuilder.InitializeCalibrationArr(CalibrationArrY, eeMemoryData.calibrationValueY);
    CalibrationY.PageStringArr[0] = strBuilder.GetCalibrationString(CalibrationArrY);
    NavigateToNextPage(&PageIndex);
  }
  else if (okButtonLevel == 2) {
    eeMemoryData.calibrationValueY = strBuilder.CalcCalibration(CalibrationArrY);
    eeMemoryAccess.SaveData(eeMemoryData);
  }
  else if (cancelButton.IsPressed()) {
    StopMotors();
    strBuilder.InitializeCalibrationArr(CalibrationArrY, eeMemoryData.calibrationValueY);
    CalibrationY.PageStringArr[0] = strBuilder.GetCalibrationString(CalibrationArrY);
    NavigateToFirstPage();
  }
  else if (upButton.IsPressed()) {
    cursorXPos = CalibrationY.GetCursorXPos();
    if (CalibrationArrY[cursorXPos] < 9) {
      CalibrationArrY[cursorXPos] += 1;
    }
    else {
      CalibrationArrY[cursorXPos] = 0;
    }
    CalibrationY.PageStringArr[0] = strBuilder.GetCalibrationString(CalibrationArrY);
    CalibrationY.UpdateScreen(lcd);
    CalibrationY.PageCalibrationValue = strBuilder.CalcCalibration(CalibrationArrY);
  }
  else if (downButton.IsPressed()) {
    cursorXPos = CalibrationY.GetCursorXPos();
    if (CalibrationArrY[cursorXPos] > 0) {
      CalibrationArrY[cursorXPos] -= 1;
    }
    else {
      CalibrationArrY[cursorXPos] = 9;
    }
    CalibrationY.PageStringArr[0] = strBuilder.GetCalibrationString(CalibrationArrY);
    CalibrationY.UpdateScreen(lcd);
    CalibrationY.PageCalibrationValue = strBuilder.CalcCalibration(CalibrationArrY);
  }
  else if (rightButton.IsPressed()) {
    CalibrationY.MoveCursorX(lcd, 1);
  }
  else if (leftButton.IsPressed()) {
    CalibrationY.MoveCursorX(lcd, -1);
  }
}
#endif
