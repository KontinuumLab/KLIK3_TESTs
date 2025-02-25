

//----------------- LOAD ALL VARIABLES FROM EEPROM ------------------ //

int address;
uint16_t tempValue;

void loadSettings(){
  int i;
  address = 0;

  
  for (i = 0; i < 9; i++) {
    mprReadingsMin[i] = EEPROM.read(address);
    address += 2;
  }
  for (i = 0; i < 9; i++) {
    mprReadingsMax[i] = EEPROM.read(address);
    address += 2;
  }
  
  for (i = 0; i < 27; i++) {
    scales[i] = EEPROM.read(address);
    address += 2;
  }
    delay(500);
  Serial.println("Settings loaded");
}



//----------------- SAVE ALL VARIABLES TO EEPROM ------------------ //

void saveSettings(){
  int i;
  address = 0;
  analogWrite(calLED, ledBrightness);
  analogWrite(scaleLED1, ledBrightness);
  analogWrite(scaleLED2, ledBrightness);
  analogWrite(scaleLED3, ledBrightness);
  
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
  
  for (i = 0; i < 27; i++) {
    EEPROM.put(address, scales[i]);
    EEPROM.commit();
    address += 2;
  }

  delay(2000);
  analogWrite(calLED, 0);
  analogWrite(scaleLED1, 0);
  analogWrite(scaleLED2, 0);
  analogWrite(scaleLED3, 0);
  Serial.println("Settings saved");
}
