/*
  a_utility.ino: Colour matching algorithm, Low-pass filter for ultrasonic sensor & on-board eeprom to store calibrated colours
*/

#define RED_THRESHOLD 200                 // Raw RED threshold 
#define RED_ORANGE_THRESHOLD 120          // Raw GREEN threshold to differentiate b/w red and orange
#define PURPLE_GREENBLUE_THRESHOLD 1.2    // Ratio of red and green to differentiate b/w purple and green/blue
#define WHITE_THRESHOLD 200

int match_color() {
  int red = currentColor[R], green = currentColor[G], blue = currentColor[B];
  if (red > WHITE_THRESHOLD and green > WHITE_THRESHOLD and blue > WHITE_THRESHOLD) return C_WHITE;
  
  if (red > RED_THRESHOLD) { // Either RED or ORANGE
    if (green > RED_ORANGE_THRESHOLD) return C_ORANGE;
    else return C_RED;
    
  } else {
    float small = min(red, green);
    float large = max(red, green);
    if (small <= 0) small = 1; // Handle division by 0
    float diffGreenRed = large / small;
    if (diffGreenRed < 0) diffGreenRed = -diffGreenRed;

    if (red >= green or diffGreenRed < PURPLE_GREENBLUE_THRESHOLD) 
      return C_PURPLE;
    else {
      if (green > blue) return C_GREEN;
      else return C_BLUE;
    }
  }
}

/* Apply low_pass filter to smoothen PID input */
float low_pass_alpha = 0.05;    // ...
float initial_threshold = 3.0;  // To account for extreme changes in value
float prev_signal = 0.0; 

float apply_low_pass_filter(float distance_cm) {
  float filtered_signal;

  if (abs(distance_cm - prev_signal) > initial_threshold) filtered_signal = distance_cm;      // Account for sudden change; when there's no wall use the raw value to avoid gradual correction of the filtered signal which may cause delay in input to PID
  else filtered_signal = (1-low_pass_alpha) * prev_signal + (low_pass_alpha * distance_cm);   // Smoothen the signal
  
#ifndef DEBUG_FILTER
  Serial.print("Filtered: ");
  Serial.println(filtered_signal);
#endif

  prev_signal = filtered_signal;
  return filtered_signal;
}

/* Arduino On-board memory to store calibrated RGB values */
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
  
  char firstCheck = EEPROM.read(eeAddress); 
  char secondCheck = EEPROM.read(eeAddress + 1);
  char thirdCheck = EEPROM.read(eeAddress + 2);

  // Check sum equivalent: Ensure EEPROM reads correct color range values
  if (firstCheck != 'J' or secondCheck != 'B' or thirdCheck != 'M') {
    Serial.println("Issue retrieving value from EEPROM! Please run calibration again!");
    while(true);
  }
}
