#ifndef MEMORY_ACCESS_H_
#define MEMORY_ACCESS_H_

#include <Arduino.h>
#include <EEPROM.h>

struct MemoryData {
  short tablePosX;
  short tablePosY;
  unsigned long motorStepsX;
  unsigned long motorStepsY;
  int calibrationValueX;
  int calibrationValueY;
  float distanceValue;
  short eeAddress = 0;
};
class MemoryAccess {
  public:
    void SaveData(MemoryData memData) {
      EEPROM.put(memData.eeAddress, memData);
    }
    void GetData(MemoryData memData){
      EEPROM.get(memData.eeAddress, memData);
    }
    void SaveTablePos(MemoryData memData){
      EEPROM.put(memData.eeAddress, memData.tablePosX);
      int nextAddr = memData.eeAddress + sizeof(short);
      EEPROM.put(nextAddr, memData.tablePosY);
    }
    void SaveMotorsPos(MemoryData memData){
      int nextAddr = memData.eeAddress + sizeof(short)*2;
      EEPROM.put(nextAddr, memData.motorStepsX);
      nextAddr += sizeof(long);
      EEPROM.put(nextAddr, memData.motorStepsY);
    }
};

#endif
