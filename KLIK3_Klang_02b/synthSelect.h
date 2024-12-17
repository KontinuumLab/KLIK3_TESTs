
int instBtnPressed = 0;

int currentSynth = 0;

void synthSelect(){
  int i;
  int j;
  uint16_t tempVal;
  unsigned long timer = millis();
  while(digitalRead(instBtn) == 0){
    delay(10);
  }


  Sampler_AllNotesOff();

  Serial.printf("setup done!\n");
  // From Marcel:
      //WavToSmpl_FileToSingleNote(FS_ID_LITTLEFS, "/PappRohrSample.wav", W2S_ALL_NOTES);

  if(currentSynth == 2){
    currentSynth = 0;
    Sampler_ClearAllSamples();
    WavToSmpl_FileToSingleNote(FS_ID_LITTLEFS, "/F3_16bit_klik3.wav", W2S_ALL_NOTES);
  }
  else if(currentSynth == 0){
    currentSynth = 1;
    Sampler_ClearAllSamples();
    WavToSmpl_FileToSingleNote(FS_ID_LITTLEFS, "/hang-drum-3_klik3.wav", W2S_ALL_NOTES);
  }
  else if(currentSynth == 1){
    currentSynth = 2;
    Sampler_ClearAllSamples();
    SF2ToSmpl_LoadAllSamplesFromSF(FS_ID_LITTLEFS, "/YoshiPartExport.sf2");
  }
      
      //SF2ToSmpl_LoadAllSamplesFromSF(FS_ID_LITTLEFS, "/Trumpet.sf2");
      Sampler_InstrumentDone();
  //-----------------------
}
