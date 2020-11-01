#ifndef STRING_BUILDER_H_
#define STRING_BUILDER_H_
class StringBuilder {

  public:
    String GetCalibrationString(short calibrationArr[]) {
      String str = "";
      for (int ii = 0; ii < 4; ii++) {
        str += String(calibrationArr[ii]);
      }
      return str;
    }
    int CalcCalibration(short calibrationArr[]) {
      int calibration = 0;
      int ii2 = 1;
      for (int ii = 3; ii >= 0; ii--, ii2 *= 10) {
        calibration += calibrationArr[ii] * ii2;
      }
      return calibration;
    }

    void InitializeCalibrationArr (short calibrationArr[] , int calibration) {
      int ii2 = 10;
      for (int ii = 3; ii >= 0; ii--, ii2 *= 10) {
        calibrationArr[ii] = (calibration % ii2) / (ii2 / 10);
      }
    }

    String GetDistanceString(short distanceArr[]) {
      String str = "";
      for (int ii = 0; ii < 6; ii++) {
        str += String(distanceArr[ii]);
        if (ii == 3)
          str += ".";
      }
      return str;
    }

    float CalcDistance(short distanceArr[]) {
      float distance = 0;
      float ii2 = 1;
      for (int ii = 3; ii >= 0; ii--, ii2 *= 10) {
        distance += distanceArr[ii] * ii2;
      }
      ii2 = 10;
      for (int ii = 4; ii <= 5; ii++, ii2 *= 10) {
        distance += distanceArr[ii] / ii2;
      }
      return distance;
    }
    void InitializeDistanceArr (short distanceArr[] , float distance) {
      int ii2 = 10;
      for (int ii = 3; ii >= 0; ii--, ii2 *= 10) {
        distanceArr[ii] = ((int)distance % ii2) / (ii2 / 10);
      }
      ii2 = 10;
      for (int ii = 5; ii >= 4; ii--, ii2 *= 10) {
        distanceArr[ii] = ((int)(distance * 100) % ii2) / (ii2 / 10);
      }
    }
};

#endif