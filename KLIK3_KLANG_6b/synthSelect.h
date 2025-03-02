#include "wav_to_sampler.h"
#include "sf_to_sampler.h"


int instBtnPressed = 0;

void selectSample(){
  if(scaleBtn1Pressed == 1){
    selectedSynth -= 1;
  }
  else if(scaleBtn2Pressed == 1){
    selectedSynth += 1;
  }
  if(selectedSynth >= 3){
    selectedSynth = 2;
  }
  else if(selectedSynth < 0){
    selectedSynth = 0;
  }
}

void synthSelect(int synth){
  int i;
  int j;
  uint16_t tempVal;
  unsigned long timer = millis();
  // while(digitalRead(instBtn) == 0){
  //   delay(10);
  // }


  Sampler_AllNotesOff();

  //Serial.println("setup done!\n");
  // From Marcel:
      //WavToSmpl_FileToSingleNote(FS_ID_LITTLEFS, "/PappRohrSample.wav", W2S_ALL_NOTES);

  if(synth == 2){
    Sampler_ClearAllSamples();
    WavToSmpl_FileToSingleNote(FS_ID_LITTLEFS, "/F3_16bit_klik3.wav", W2S_ALL_NOTES);
  }
  else if(synth == 0){
    Sampler_ClearAllSamples();
    WavToSmpl_FileToSingleNote(FS_ID_LITTLEFS, "/hang-drum-3_klik3.wav", W2S_ALL_NOTES);
  }
  else if(synth == 1){
    Sampler_ClearAllSamples();
    SF2ToSmpl_LoadCompleteSoundFont(FS_ID_SD_MMC, "/Guitars-Universal-V1.sf2");
  }
      
      
  Sampler_InstrumentDone();
  //-----------------------
}
