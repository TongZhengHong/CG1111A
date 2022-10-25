#define RED_THESHOLD 170 // Absolute raw ldr value
#define RED_ORANGE_THRESHOLD 2.8 // Ratio of R to G = 2.8
#define PURPLE_BLUEGREEN_THRESHOLD 1.6 // Ratio of R to G = 1.6
#define WHITE_THRESHOLD 200

int match_color() {
  int red = currentColor[R], green = currentColor[G], blue = currentColor[B];
  if (red > WHITE_THRESHOLD and green > WHITE_THRESHOLD and blue > WHITE_THRESHOLD) return C_WHITE;
  
  if (red > RED_THESHOLD) { // Either RED or ORANGE
    float small = min(red, green);
    float large = max(red, green);
    float diffGreenRed = large / small;

    if (abs(diffGreenRed) > RED_ORANGE_THRESHOLD) return C_RED;
    else return C_ORANGE;
    
  } else {
    float small = min(red, green);
    float large = max(red, green);
    float diffGreenRed = large / small;

    if (abs(diffGreenRed) > PURPLE_BLUEGREEN_THRESHOLD) {
      if (green > blue) return C_GREEN;
      else return C_BLUE;
      
    } else return C_PURPLE;
  }
}

float low_pass_alpha = 0.05;
float initial_threshold = 3.0;
float prev_signal = 0.0; 

float apply_low_pass_filter(float distance_cm) {
  float filtered_signal;

  if (abs(distance_cm - prev_signal) < initial_threshold) filtered_signal = distance_cm;
  else filtered_signal = (1-low_pass_alpha) * prev_signal + (low_pass_alpha * distance_cm);
  
#ifndef DEBUG_FILTER
  Serial.print("Filtered: ");
  Serial.println(filtered_signal);
#endif

  prev_signal = filtered_signal;
  return filtered_signal;
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

//  Serial.println("Retrieving color values from EEPROM...");
//  for (int i = 0; i < 5; i++) {
//    for (int c = 0; c < 3; c++) {
//      EEPROM.get(eeAddress, colorsArray[i][c]);
//      eeAddress += sizeof(int);
//
//      Serial.print(colorsArray[i][c]);
//      Serial.print(",");
//    }
//    Serial.println("");
//  }
  
  char firstCheck = EEPROM.read(eeAddress); 
  char secondCheck = EEPROM.read(eeAddress + 1);
  char thirdCheck = EEPROM.read(eeAddress + 2);

  if (firstCheck != 'J' or secondCheck != 'B' or thirdCheck != 'M') {
    Serial.println("Issue retrieving value from EEPROM! Please run calibration again!");
    while(true);
  }
}
