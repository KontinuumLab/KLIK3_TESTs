
// void samplerPlay(int channel, int note, int velocity){
//   if(currentSynth == 0){
//     Sampler_NoteOn(0, scale[i]+45, 127);
//   }
//   else if(currentSynth == 1){
//     Sampler_NoteOn(0, scale[i]+45, 127);
//   }
//   else if(currentSynth == 2){

//   }
// }

int amplitudeInt;

void klikInstrument() {
  
  lastMPRbinaryA =  MPRbinaryA;
  MPRbinaryA = capA.touched();
  int i;
  int synthCont = 0;
  //int j;
  if(lastMPRbinaryA != MPRbinaryA){
    
    for(i = 0; i < 9; i++){
      //j = i + 2;
      lastMprReadings[i] = mprReadings[i];
      //What kind of synth sound? - percussive or continuous?
      if(currentSynth == 0 || currentSynth == 1){
        synthCont = 0;
      }
      else{
        synthCont = 1;
      }
      synthCont = 0; //skip synthCont consideration:

    //Execute percussive synth
      if(synthCont == 0){
        if(bitRead(MPRbinaryA, i) == 1){
          if(bitRead(lastMPRbinaryA, i) == 0){
            mprReadings[i] = capA.filteredData(i);
            Serial.print(mprReadings[i]);
            mprReadings[i] = constrain(mprReadings[i],mprReadingsMax[i], mprReadingsMin[i]);
            
            // amplitudeInt = map(mprReadings[i], mprReadingsMin[i], mprReadingsMax[i], 1, 6);
    //        constrain(amplitudeInt, 0, 10);
    //        amplitude = float(amplitudeInt);
            // amplitude = amplitudeInt/10.0;
    //        Serial.print(i);
    //        Serial.print(" - ");
    //        Serial.println(amplitude);
            // noteOn(scale[i], amplitude);


            int note = i + 45;
            if(audioOn == 1){
              //Sampler_NoteOn(0, scale[i]+45, 127);
              Sampler_NoteOn(0, note, 127);
              Serial.print(" - ");
              Serial.println(scale[i]+45);
            }
            else if(audioOn == 0){
              MIDI.sendNoteOn(note, 127, 1);
            }
          //touchedKeys[i] = bitRead(MPRbinaryA, i);
          }
        }
      }
      else{ //Execute continuous synth
        if(bitRead(MPRbinaryA, i) == 1){
          //if(bitRead(lastMPRbinaryA, i) == 0){
            mprReadings[i] = capA.filteredData(i);
            
            mprReadings[i] = constrain(mprReadings[i], mprReadingsMin[i], mprReadingsMax[i]);
            Serial.print(mprReadings[i]);

            amplitudeInt = map(mprReadings[i], mprReadingsMin[i], mprReadingsMax[i], 127, 0);
    //        amplitude = float(amplitudeInt);
            // amplitude = amplitudeInt/10.0;
    //        Serial.print(i);
    //        Serial.print(" - ");
    //        Serial.println(amplitude);
            // noteOn(scale[i], amplitude);
            int note = i + 45;
            if(audioOn == 1){
              Sampler_NoteOn(0, scale[i]+45, amplitudeInt);
              Serial.print(" - ");
              Serial.println(scale[i]+45);
            }
            else if(audioOn == 0){
              MIDI.sendNoteOn(note, amplitudeInt, 1);
            }
          //touchedKeys[i] = bitRead(MPRbinaryA, i);
          //}
          // else{
          //   amplitudeInt = map(mprReadings[i], mprReadingsMin[i], mprReadingsMax[i], 0, 127);
          //   constrain(amplitudeInt, 0, 127);
          // }
          if(currentSynth == 2){
            Sampler_NoteOn(0, scale[i]+45, 0);
          }
          mprReadings[i] = 0;
        }
      }
      //else(mprReadings[i] = 0);
    }
  }
//  else if(btnPressed == 1 && lastBtnPressed == 0){
//  noteOff();
//  }

}