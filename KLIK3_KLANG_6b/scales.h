

int midiNotes[28] = {44, };

void factoryReset(){
  int i;
  int j;
  for(i = 0; i < 27; i++){
    scales[i] = scalesFactory[i];
  }
  for(i = 0; i < 9; i++){
    j = i + (currentScale * 9);
    scale[i] = scales[j]; //Get new notes from new scale
  }
  for(i = 0; i < 6; i++){
    // analogWrite(scaleLED1, ledBrightness);
    // analogWrite(scaleLED2, ledBrightness);
    // analogWrite(scaleLED3, ledBrightness);
    delay(200);
    // analogWrite(scaleLED1, 0);
    // analogWrite(scaleLED2, 0);
    // analogWrite(scaleLED3, 0);
    delay(200);
  }
}

void scaleAdjust(){
  int i;
  int j;
  int newNoteValue;



//##############################
// change notes:
//#############################
  if(MPRbinaryA != 0){ // Some strike zone is being touched
    if(scaleBtn1Pressed == 1){ //Button 1 has just been Val
      //analogWrite(scaleLED1, ledBrightness);
      for(i = 0; i < 9; i++){
        if(bitRead(MPRbinaryA, i) == 1){
          newNoteValue = scale[i] - 1;
          if(newNoteValue >= 0){
            scale[i] = newNoteValue;
          }
        }
      }
    }
    else if(scaleBtn2Pressed == 1){ //Button 2 has just been Val
      //analogWrite(scaleLED3, ledBrightness);
      for(i = 0; i < 9; i++){
        if(bitRead(MPRbinaryA, i) == 1){
          newNoteValue = scale[i] + 1;
          if(newNoteValue <= 128){
            scale[i] = newNoteValue;
          }
        }
      }
    }

  }

//##############################
// switch scale:
//#############################

  else if(MPRbinaryA == 0){ // No strike zone is being touched
    if(scaleBtn1Pressed == 1){

      for(i = 0; i < 9; i++){
        j = i + (currentScale * 9);
        scales[j] = scale[i]; //Put current notes in the current scale
      }
      if(currentScale != 0){
        currentScale = currentScale - 1;
      }
      for(i = 0; i < 9; i++){
        j = i + (currentScale * 9);
        scale[i] = scales[j]; //Get new notes from new scale
      }
      if(currentScale == 0){
        //analogWrite(scaleLED2, 0);
        //analogWrite(scaleLED1, ledBrightness);
        //ledPinTimer = millis();
        //scaleLedOn = 1;
      }
      else if(currentScale == 1){
        //analogWrite(scaleLED3, 0);
        //analogWrite(scaleLED2, ledBrightness);
        //ledPinTimer = millis();
        //scaleLedOn = 1;
      }
    }

    if(scaleBtn2Pressed == 1){

      for(i = 0; i < 9; i++){
        j = i + (currentScale * 9);
        scales[j] = scale[i]; //Put current notes in the current scale
      }
      currentScale = currentScale + 1;
      currentScale = constrain(currentScale, 0, 2);
      for(i = 0; i < 9; i++){
        j = i + (currentScale * 9);
        scale[i] = scales[j]; //Get new notes from new scale
      }
      if(currentScale == 1){
        //analogWrite(scaleLED1, 0);
        //analogWrite(scaleLED2, ledBrightness);
        //ledPinTimer = millis();
        //scaleLedOn = 1;
      }
      else if(currentScale == 2){
        //analogWrite(scaleLED2, 0);
        //analogWrite(scaleLED3, ledBrightness);
        //ledPinTimer = millis();
        //scaleLedOn = 1;
      }     
    }      

  }
}