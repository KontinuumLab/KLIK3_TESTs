

//----------------- LOAD ALL VARIABLES FROM EEPROM ------------------ //

int address;
uint16_t tempValue;

void loadScales(){
  int i;
  address = 0;

  for (i = 0; i < 27; i++) {
    scales[i] = EEPROM.read(address);
    address += 2;
  }
    delay(500);
  Serial.println("Settings loaded");
}


void loadCalibration(){
  int i;
  address = 60;

  
  for (i = 0; i < 9; i++) {
    mprReadingsMin[i] = EEPROM.read(address);
    address += 2;
  }
  for (i = 0; i < 9; i++) {
    mprReadingsMax[i] = EEPROM.read(address);
    address += 2;
  }

}


void loadSettings(){
  loadScales();
  loadCalibration();
}



void saveScales(){
  int i;
  address = 0;

  for (i = 0; i < 27; i++) {
    EEPROM.put(address, scales[i]);
    EEPROM.commit();
    address += 2;
  }

}


void saveCalibration(){
  int i;
  address = 60;
  //analogWrite(calLED, ledBrightness);
  //analogWrite(scaleLED1, ledBrightness);
  // analogWrite(scaleLED2, ledBrightness);
  // analogWrite(scaleLED3, ledBrightness);
  
  for (i = 0; i < 9; i++) {
    EEPROM.put(address, mprReadingsMin[i]);
    EEPROM.commit();
    address += 2;
  }
  for (i = 0; i < 9; i++) {
    EEPROM.put(address, mprReadingsMax[i]);
    EEPROM.commit();
    address += 2;
  }

}
//----------------- SAVE ALL VARIABLES TO EEPROM ------------------ //

void saveSettings(){
  saveScales();
  saveCalibration();
}
