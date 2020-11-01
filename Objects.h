#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <LiquidCrystal.h>

#include "Button.h"
#include "PinMap.h"
#include "LCDPage.h"
#include <AccelStepper.h>
#include "StringBuilder.h"
#include "MemoryAccess.h" 
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4 , LCD_D5 , LCD_D6 , LCD_D7);

//---- Buttons -------
Button upButton(UP_BUTTON_PIN);
Button downButton(DOWN_BUTTON_PIN);
Button rightButton(RIGHT_BUTTON_PIN);
Button leftButton(LEFT_BUTTON_PIN);
Button okButton(OK_BUTTON_PIN);
Button cancelButton(CANCEL_BUTTON_PIN);
Button iRX_Button(IR01_PIN);
Button iRY_Button(IR02_PIN);

//---- LCD Pages ----
LCDPage MainPage;
LCDPage DistancePage;
LCDPage HomingPage;
LCDPage CalibrationX;
LCDPage CalibrationY;

const int PagesSize = 5;
LCDPage* Pages[PagesSize] = {&MainPage, &HomingPage, &DistancePage, &CalibrationX, &CalibrationY};

//--- Stepper Motors ---
AccelStepper stepperX(1, STEPPER_X_CLOCK, STEPPER_X_DIRECTION);
AccelStepper stepperY(1, STEPPER_Y_CLOCK, STEPPER_Y_DIRECTION);

//---String & Array---
StringBuilder strBuilder;

//--- EEPROM Memory Access ---
MemoryAccess eeMemoryAccess;
MemoryData eeMemoryData;
#endif
