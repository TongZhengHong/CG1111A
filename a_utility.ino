void checkRightWall() {
  
}

void checkLeftWall() {
  
}

void solveChallenge() {
  
}

void read_eeprom() {
  int eeAddress = 0;
  
  Serial.println("Retrieving balance values from EEPROM...");
  for (int b = 0; b < 3; b++) {
    for (int c = 0; c < 3; c++) {
      EEPROM.get(eeAddress, balanceArray[b][c]);
      eeAddress += sizeof(int);
      
      Serial.print(balanceArray[b][c]);
      Serial.print(",");
    }
    Serial.println("");
  }

  Serial.println("Retrieving color values from EEPROM...");
  for (int i = 0; i < 5; i++) {
    for (int c = 0; c < 3; c++) {
      EEPROM.get(eeAddress, colorsArray[i][c]);
      eeAddress += sizeof(int);

      Serial.print(colorsArray[i][c]);
      Serial.print(",");
    }
    Serial.println("");
  }
  
  char firstCheck = EEPROM.read(eeAddress); 
  char secondCheck = EEPROM.read(eeAddress + 1);
  char thirdCheck = EEPROM.read(eeAddress + 2);

  if (firstCheck != 'J' or secondCheck != 'B' or thirdCheck != 'M') {
    Serial.println("Issue retrieving value from EEPROM! Please run calibration again!");
    while(true);
  }
}
