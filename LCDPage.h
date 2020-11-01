#ifndef LCDPAGE_H_
#define LCDPAGE_H_

#include <Arduino.h>
#include <LiquidCrystal.h>

class LCDPage {
  private:
    int cursorXPos = 0 ;
    int cursorYPos = 0 ;
  public:
    String PageStringArr[2];
    bool IsCursorShown = true ;
    float PageDistanceValue ;
    int PageCalibrationValue ;
    int MaxPageWidth = 16;
    int MaxPageHeight = 2;
    bool IsActive = false ;
    LCDPage() {

    }
    //LCDPage() {
    //IsActive = isActive;
    //IsCursorShown = isCursorShown;
    //Initialize(lcd);
    //}

    void InitializeCursor(LiquidCrystal lcd) {
      if (IsCursorShown) {
        lcd.cursor();
      }
      else {
        lcd.noCursor();
      }
      lcd.setCursor(cursorXPos, cursorYPos);
    }

    void Navigate(LiquidCrystal lcd)
    {
      PrintString(lcd);
      InitializeCursor(lcd);
      IsActive = true;
    }
    void PrintString(LiquidCrystal lcd) {
      lcd.clear();
      for (int ii = 0; ii <= 1; ii++) {
        lcd.setCursor(0, ii);
        lcd.print(PageStringArr[ii]);
      }
    }
    void UpdateScreen(LiquidCrystal lcd){
      PrintString(lcd);
      InitializeCursor(lcd);
    }
    int GetCursorXPos() {
      return cursorXPos;
    }
    int GetCursorYPos() {
      return cursorYPos;
    }
    void SetCursorXPos(LiquidCrystal lcd, int newXPos) {
      if (newXPos > MaxPageWidth - 1) {
        cursorXPos = MaxPageWidth - 1;
      } else if (cursorXPos + newXPos < 0) {
        cursorXPos = 0;
      }
      else{
        cursorXPos = newXPos;
      }
      lcd.setCursor(cursorXPos, cursorYPos);
    }
    void SetCursorYPos(LiquidCrystal lcd,int newYPos) {
      if (newYPos > MaxPageHeight - 1) {
        cursorYPos = MaxPageHeight - 1;
      } else if (cursorYPos + newYPos < 0) {
        cursorYPos = 0;
      }
      else{
        cursorYPos = newYPos;
      }
      lcd.setCursor(cursorXPos, cursorYPos);
    }
    void MoveCursorX(LiquidCrystal lcd, int Steps) {
      SetCursorXPos(lcd, GetCursorXPos() + Steps);
    }
    void MoveCursorY(LiquidCrystal lcd, int Steps) {
      SetCursorYPos(lcd, GetCursorYPos() + Steps);
    }
};

#endif
