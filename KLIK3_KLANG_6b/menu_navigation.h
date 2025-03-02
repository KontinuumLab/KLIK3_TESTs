int debounceTime = 1;

void readBtns(){
  lastScaleBtn1Val = scaleBtn1Val;
  if(!digitalRead(scaleBtn1) != lastScaleBtn1Val){
    delay(debounceTime);
    scaleBtn1Val = !digitalRead(scaleBtn1);
  }
  if(scaleBtn1Val == 1 && lastScaleBtn1Val == 0){
    scaleBtn1Pressed = 1;
    scaleBtn1Released = 0;
  }
  else if(scaleBtn1Val == 0 && lastScaleBtn1Val == 1){
    scaleBtn1Pressed = 0;
    scaleBtn1Released = 1;
  }
  else{
    scaleBtn1Pressed = 0;
    scaleBtn1Released = 0;
  }


  lastScaleBtn2Val = scaleBtn2Val;
  if(!digitalRead(scaleBtn2) != lastScaleBtn2Val){
    delay(debounceTime);
    scaleBtn2Val = !digitalRead(scaleBtn2);
  }
  if(scaleBtn2Val == 1 && lastScaleBtn2Val == 0){
    scaleBtn2Pressed = 1;
    scaleBtn2Released = 0;
  }
  else if(scaleBtn2Val == 0 && lastScaleBtn2Val == 1){
    scaleBtn2Pressed = 0;
    scaleBtn2Released = 1;
  }
  else{
    scaleBtn2Pressed = 0;
    scaleBtn2Released = 0;
  }


  lastSelBtnVal = selBtnVal;
  if(!digitalRead(selBtn) != lastSelBtnVal){
    delay(debounceTime);
    selBtnVal = !digitalRead(selBtn);
  }
  if(selBtnVal == 1 && lastSelBtnVal == 0){
    selBtnPressed = 1;
    selBtnReleased = 0;
  }
  else if(selBtnVal == 0 && lastSelBtnVal == 1){
    selBtnPressed = 0;
    selBtnReleased = 1;
  }
  else{
    selBtnPressed = 0;
    selBtnReleased = 0;
  }
}


void selectMemoryMenuItem(){
  if(scaleBtn1Pressed == 1){
    memoryMenuItem -= 1;
  }
  else if(scaleBtn2Pressed == 1){
    memoryMenuItem += 1;
  }
  if(memoryMenuItem >= 3){
    memoryMenuItem = 2;
  }
  else if(memoryMenuItem < 0){
    memoryMenuItem = 0;
  }

}

void klikMenu(){
  int i;
  readBtns();
  if(currentMenu == 0){
    showBlankDisplay();
    if(selBtnPressed == 1){
      selBtnPressed = 0;
      menuTimer = millis();
      while(selBtnReleased != 1){
        readBtns();
        if(millis() - menuTimer > 800){
          currentMenu = 0;
        }
        else{
          currentMenu = 1;
        }
      }
      selBtnReleased = 0;
    }
  }

  if(currentMenu == 0){
    
  }
  else if(currentMenu == 1){
    scaleAdjust();
    showScaleMenu();
    if(selBtnPressed == 1){
      selBtnPressed = 0;
      menuTimer = millis();
      while(selBtnReleased != 1){
        readBtns();
        if(millis() - menuTimer > 800){
          currentMenu = 0;
          showBlankDisplay();
        }
        else{
          selectedSynth = currentSynth;
          currentMenu = 2;
        }
      }
      selBtnReleased = 0;
    }
  }


  else if(currentMenu == 2){
    selectSample();
    showSampleMenu();
    if(selBtnPressed == 1){
      selBtnPressed = 0;
      menuTimer = millis();
      while(selBtnReleased != 1){
        readBtns();
        if(millis() - menuTimer > 800){
          currentSynth = selectedSynth;
          showSampleLoadingMenu();
          delay(500);
          synthSelect(currentSynth);
          currentMenu = 0;
          showBlankDisplay();
        }
        else{
          currentMenu = 3;
        }
      }
      selBtnReleased = 0;
    }
  }



  else if(currentMenu == 3){
    selectMemoryMenuItem();
    showMemoryMenu();
    if(selBtnPressed == 1){
      selBtnPressed = 0;
      menuTimer = millis();
      while(selBtnReleased != 1){
        readBtns();
        if(millis() - menuTimer > 800){
          
          if(memoryMenuItem == 0){
            showMemoryLoadingMenu(0);
            delay(500);
            saveScales();
          }
          else if(memoryMenuItem == 1){
            showMemoryLoadingMenu(1);
            delay(500);
            saveCalibration();
          }
          else if(memoryMenuItem == 2){
            showMemoryLoadingMenu(2);
            delay(500);
            for(i = 0; i < 27; i++){
              scales[i] = scalesFactory[i];
            }
          }
          currentMenu = 0;
          showBlankDisplay();
        }
        else{
          currentMenu = 4;
          calibrationMenuItem = 0;
        }
      }
      selBtnReleased = 0;
    }
  }



  else if(currentMenu == 4){
    if(calibrationMenuItem == 0){
      showCalibrationMenu();
      if(selBtnPressed == 1){
        selBtnPressed = 0;
        menuTimer = millis();
        while(selBtnReleased != 1){
          readBtns();
          if(millis() - menuTimer > 800){
            calibrationMenuItem = 1;
            selBtnPressed = 0;
            showCalibratingMenu();
            currentMenu = 4;
            for(i = 0; i < 9; i++){
              mprReadingsMin[i] = 0;
              mprReadingsMax[i] = 2048;
            }
          }
          else{
            currentMenu = 0;
          }
        }
        selBtnReleased = 0;
      }
    }
    else{ //calibrationMenuItem = 1
      readBtns();
      while(selBtnPressed != 1){
        readBtns();
        calibration();
        showCalibratingMenu();
      }
      calibrationMenuItem = 0;
      currentMenu = 0;
      showBlankDisplay();
    }
  }
}
