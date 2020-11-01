#ifndef BUTTON_H_
#define BUTTON_H_

class Button {
  public:
    bool btnStateFlag = 0;
    short btnPressFlag = 0;
    bool lastButtonState ;
    unsigned long currentTime;
    unsigned long lastDebounceTime = 0;
    short btnHoldPressFlag = 0;
    unsigned long lastPressedTime = 0;
    bool btnState ;

    short Pin;
    unsigned int DebounceDelay = 10;
    unsigned int LongPressDelay = 1000;
    //bool IsPressed = 0;
    //short HoldLevel = 0;

    Button(int pinNumber) {
      Initialize(pinNumber);
    }

    void Initialize(short pinNumber) {
      Pin = pinNumber;
      pinMode(pinNumber , INPUT);
      lastButtonState = digitalRead(pinNumber);
    }

    bool GetState() {
      currentTime = millis();
      if (digitalRead(Pin) != lastButtonState && btnStateFlag == 0) {
        lastDebounceTime = currentTime;
        btnStateFlag = 1;
      }
      else if ((currentTime - lastDebounceTime) > DebounceDelay && btnStateFlag == 1) {
        lastButtonState = digitalRead(Pin);
        btnStateFlag = 0;
        return lastButtonState;
      } else {
        return lastButtonState;
      }
    }


    bool IsPressed() {
      btnState = GetState();
      if (btnState && btnPressFlag == 0) {
        btnPressFlag = 1;
        return true;
      }
      else if (!btnState && btnPressFlag == 1) {
        btnPressFlag = 2;
      }else if(!btnState && btnPressFlag == 2){
        btnPressFlag = 0;
      }
      return false;
    }

    short GetHoldLevel() {
      btnState = GetState();

      if (btnState && btnHoldPressFlag == 0) {
        lastPressedTime = currentTime;
        btnHoldPressFlag = 1;
      }
      else if ((currentTime - lastPressedTime) < LongPressDelay && !btnState && btnHoldPressFlag == 1 ) {
        lastPressedTime = currentTime;
        btnHoldPressFlag = 3;
        return 1;
      }
      else if ((currentTime - lastPressedTime) >= LongPressDelay && btnState && btnHoldPressFlag == 1 ) {
        btnHoldPressFlag = 4;
        return 2;
      }
      else if (!btnState && btnHoldPressFlag == 4) {
        btnHoldPressFlag = 3;
      }
      else if (!btnState && btnHoldPressFlag == 3) {
        btnHoldPressFlag = 0;
      }
      return 0;
    }
};
#endif
