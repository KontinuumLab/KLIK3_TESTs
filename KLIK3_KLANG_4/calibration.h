
int calBtnPressed = 0;
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
  while(digitalRead(calBtn) == 0){
    delay(10);
    if(millis() - 2000 > timer){
      for(i = 0; i < 9; i++){ 
        j = i + (currentScale * 9);
        scales[j] = scale[i]; //Put current notes in the current scale before saving
      }
      saveSettings();
      return;
    }
  }
  analogWrite(calLED, ledBrightness);
    // Start calibration loop:
  calBtnPressed = 0;
  for(i = 0; i < 9; i++){
    mprReadingsMin[i] = 0;
    mprReadingsMax[i] = 2048;
  }

  while(calBtnPressed == 0){
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

    
    calBtnPressed = debounce(calBtn); 
  }
  analogWrite(calLED, 0);
  for(i = 0; i < 9; i++){
    Serial.print(mprReadingsMin[i]);
    Serial.print(" - ");
    Serial.println(mprReadingsMax[i]);
  }
  while(digitalRead(calBtn) == 0){
    delay(10);
  }

  // Adjust minimum and maximum values with a buffer value:
//  for(i = 0; i < 24; i++){
//    tempVal = (mprReadingsMax[i] - mprReadingsMin[i]) / 7;
//    mprReadingsMin[i] = mprReadingsMin[i] - tempVal;
//    mprReadingsMin[i] = mprReadingsMin[i] + tempVal;
//
//  }
}