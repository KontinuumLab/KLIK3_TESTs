

int calibrating = 0;

int calibrationPhase;

unsigned int calibrationStart;
unsigned int calibrationTimer;

int debounce(int pin){
  int temp = !digitalRead(pin);
  if(temp == 1){
    delay(2);
    temp = !digitalRead(pin);
    return(temp);
  }
  else{
    return 0;
  }
}

void calibration(){
  int i;
  int j;
  uint16_t tempVal;
  unsigned long timer = millis();

  //analogWrite(calLED, ledBrightness);
    // Start calibration loop:


  MPRbinaryA = capA.touched();

  for(i = 0; i < 9; i++){
    //j = i + 2;
    if(bitRead(MPRbinaryA, i) != 0){
//      lastMux1RawVal[i] = mux1RawVal[i];
      mprReadings[i] = capA.filteredData(j);
//      mux1RawVal[i] = expFilter(mux1RawVal[i], lastMux1RawVal[i], 0.001);
      if(mprReadings[i] > mprReadingsMin[i]){
        mprReadingsMin[i] = mprReadings[i];
      }
      else if(mprReadings[i] < mprReadingsMax[i]){
        mprReadingsMax[i] = mprReadings[i];
      }
    }
  }

  // Adjust minimum and maximum values with a buffer value:
//  for(i = 0; i < 24; i++){
//    tempVal = (mprReadingsMax[i] - mprReadingsMin[i]) / 7;
//    mprReadingsMin[i] = mprReadingsMin[i] - tempVal;
//    mprReadingsMin[i] = mprReadingsMin[i] + tempVal;
//
//  }
}