void showBlankDisplay(){
  display.clearDisplay();
  display.display();
}

void drawHangRect(int x, int y, int type){
  if(type == 0){
    display.drawRect(x, y, 2, 2, SSD1306_WHITE);
  }
  else{
    display.drawRect(x-1, y-1, 4, 4, SSD1306_WHITE);
  }
}


void drawAllHangRect(){
  drawHangRect(12, 14, bitRead(MPRbinaryA, 0));
  drawHangRect(15, 22, bitRead(MPRbinaryA, 1));
  drawHangRect(9, 22, bitRead(MPRbinaryA, 2));
  drawHangRect(20, 17, bitRead(MPRbinaryA, 3));
  drawHangRect(4, 17, bitRead(MPRbinaryA, 4));
  drawHangRect(20, 11, bitRead(MPRbinaryA, 5));
  drawHangRect(4, 11, bitRead(MPRbinaryA, 6));
  drawHangRect(15, 6, bitRead(MPRbinaryA, 7));
  drawHangRect(9, 6, bitRead(MPRbinaryA, 8));
}

void drawHang(){
  display.drawLine(8, 2, 17, 2, SSD1306_WHITE);
  display.drawLine(7, 2, 0, 9, SSD1306_WHITE);
  display.drawLine(0, 10, 0, 19, SSD1306_WHITE);
  display.drawLine(0, 20, 7, 27, SSD1306_WHITE);
  display.drawLine(8, 27, 17, 27, SSD1306_WHITE);
  display.drawLine(18, 27, 25, 20, SSD1306_WHITE);
  display.drawLine(25, 19, 25, 10, SSD1306_WHITE);
  display.drawLine(25, 9, 18, 2, SSD1306_WHITE);
  drawAllHangRect();
}

void printNoteNames(int x, int y, int pos){
  int noteNamePosition;

  if(bitRead(MPRbinaryA, pos) ==1){
    display.setTextColor(BLACK, WHITE); //'inverted' text
    display.setCursor(x-1, y+1);
    display.print(" ");
    display.setCursor(x-1, y-1);
    display.print(" ");
  }
  display.setCursor(x, y);
    noteNamePosition = scale[pos] % 12;

  display.print(noteNames[noteNamePosition]);
  display.setTextColor(WHITE, BLACK); 
}

void printNoteSharps(int x, int y, int pos){
  int noteNamePosition;

  if(bitRead(MPRbinaryA, pos) ==1){
    display.setTextColor(BLACK, WHITE); //'inverted' text
    // display.setCursor(x-1, y+1);
    // display.print(" ");
    // display.setCursor(x-1, y-1);
    // display.print(" ");
  }
  display.setCursor(x, y);
    noteNamePosition = scale[pos] % 12;

  display.print(noteSharps[noteNamePosition]);
  display.setTextColor(WHITE, BLACK); 
}


void showScaleMenu() {

//############ SCALE SELECT DISPLAY: ###############  
  display.clearDisplay();
  

  display.setTextSize(1);   
  display.setTextColor(WHITE, BLACK); 
  display.setCursor(30, 2);
  display.print("SCALE:");

  if(currentScale == 0){
    display.setTextColor(BLACK, WHITE); //'inverted' text
    display.setCursor(64, 1);
    display.print("   ");
    display.setCursor(64, 3);
    display.print("   ");
  }  
    display.setCursor(64, 2);
    display.print(" 1 ");
    display.setTextColor(WHITE, BLACK); 

  if(currentScale == 1){
    display.setTextColor(BLACK, WHITE); //'inverted' text
    display.setCursor(84, 1);
    display.print("   ");
    display.setCursor(84, 3);
    display.print("   ");
  }
    display.setCursor(84, 2);
    display.print(" 2 ");
    display.setTextColor(WHITE, BLACK); 


  if(currentScale == 2){
    display.setTextColor(BLACK, WHITE); //'inverted' text
    display.setCursor(104, 1);
    display.print("   ");
    display.setCursor(104, 3);
    display.print("   ");
  }
    display.setCursor(104, 2);
    display.print(" 3 ");
    display.setTextColor(WHITE, BLACK); 



//##### NOTE NAMES: ###################


  printNoteNames(30, 24, 0);
  printNoteNames(40, 24, 1);
  printNoteNames(50, 24, 2);
  printNoteNames(60, 24, 3);
  printNoteNames(70, 24, 4);
  printNoteNames(80, 24, 5);
  printNoteNames(90, 24, 6);
  printNoteNames(100, 24, 7);
  printNoteNames(110, 24, 8);



  printNoteSharps(30, 16, 0);
  printNoteSharps(40, 16, 1);
  printNoteSharps(50, 16, 2);
  printNoteSharps(60, 16, 3);
  printNoteSharps(70, 16, 4);
  printNoteSharps(80, 16, 5);
  printNoteSharps(90, 16, 6);
  printNoteSharps(100, 16, 7);
  printNoteSharps(110, 16, 8);


  drawHang();

  display.display();   
}

void showSampleMenu() {
  display.clearDisplay();
  
  display.setTextSize(1);   
  display.setTextColor(WHITE, BLACK); 
  display.setCursor(10, 1);
  display.print("Current sample: ");
  display.setCursor(108, 1);
  display.print(currentSynth);
  display.setCursor(10, 12);
  display.print("New sample: ");
  display.setCursor(108, 12);
  display.print(selectedSynth);
  display.setCursor(0, 23);
  display.print("Long 'sel' to select");
  display.display();   

}

void showSampleLoadingMenu(){
  display.clearDisplay();
  
  display.setTextSize(1);   
  display.setTextColor(WHITE, BLACK); 
  display.setCursor(10, 2);
  display.print("Loading sample: "); 
  display.print(currentSynth);
  display.display(); 
}


void showMemoryMenu(){
  display.clearDisplay();
  
  display.setTextSize(1);   
  display.setTextColor(WHITE, BLACK); 

  display.setCursor(12, 0);
  if(memoryMenuItem == 0){
    display.setTextColor(BLACK, WHITE); 
  }
  display.print("Save All Scales");
  display.setTextColor(WHITE, BLACK); 


  display.setCursor(12, 11);
  if(memoryMenuItem == 1){
    display.setTextColor(BLACK, WHITE); 
  }
  display.print("Save Calibration");
  display.setTextColor(WHITE, BLACK); 


  display.setCursor(12, 22);
  if(memoryMenuItem == 2){
    display.setTextColor(BLACK, WHITE); 
  }
  display.print("Load Factory Scales");
  display.setTextColor(WHITE, BLACK); 

  display.display();  
}

void showMemoryLoadingMenu(int menuItem){
  display.clearDisplay();
  display.setTextSize(1);   
  display.setTextColor(WHITE, BLACK); 
  

  if(memoryMenuItem == 0){
    display.setCursor(5, 9);
    display.print("Saving custom scales"); 
    display.display(); 
  }
  if(memoryMenuItem == 1){
    display.setCursor(10, 9);
    display.print("Saving calibration"); 
    display.display(); 
  }
  if(memoryMenuItem == 2){
    display.setCursor(18, 5);
    display.print("Loading factory"); 
    display.setCursor(40, 16);
    display.print("settings");
    display.display(); 
  }
}

void showCalibrationMenu(){
  display.clearDisplay();
  
  display.setTextSize(1);   
  display.setTextColor(WHITE, BLACK); 
  display.setCursor(2, 5);
  display.print("Long press 'Sel.' to");
  display.setCursor(35, 20);
  display.print("CALIBRATE ");
  display.display();  
}

void showCalibratingMenu(){
  display.clearDisplay();

  drawHang();
  display.setTextSize(1);   
  display.setTextColor(WHITE, BLACK); 
  display.setCursor(30, 1);
  display.print("Calibrating...");
  display.setCursor(30, 12);
  display.print("Activate sensors");
  display.setCursor(30, 23);
  display.print("Then press 'sel'");
  display.display(); 
}


