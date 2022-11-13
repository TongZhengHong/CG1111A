#include <EEPROM.h>

#define D1 A2
#define D2 A3

#define LDR 0
#define LED 13 

#define RGBWait 100
#define LDRWait 10

#define WHITE 0
#define BLACK 1
#define GREY 2

#define COUNTDOWN 5
#define AVG_READING 5

int ledPins[4][2] = {
  { HIGH, HIGH },   // Red LED
  { LOW, HIGH },    // Green LED
  { HIGH, LOW },    // Blue LED
  { LOW, LOW }      // None
};

int currentColor[3] = {0,0,0};

int balance[3][3] = {
  {0, 0, 0}, // WHITE
  {0, 0, 0}, // BLACK
  {0, 0, 0}, // GREY
};

int finalColors[5][3] = {
  {0, 0, 0}, // RED
  {0, 0, 0}, // GREEN
  {0, 0, 0}, // ORANGE
  {0, 0, 0}, // PURPLE
  {0, 0, 0}, // BLUE
};

void setup() {
  Serial.begin(9600);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  setBalance();

//  getColorRanges();

  writeToEEPROM();

  Serial.println("Color calibration is done!");
}

void loop() {
  for (int c = 0; c < 3; c++) {
    digitalWrite(D1, ledPins[c][0]);
    digitalWrite(D2, ledPins[c][1]);
    delay(RGBWait);

    // Get average LDR reading
    int total = 0;
    for (int i = 0; i < AVG_READING; i++) {
      int reading = analogRead(LDR);
      total += reading;
      delay(LDRWait);
    }

    currentColor[c] =  total / AVG_READING;
    currentColor[c] = ((float) (currentColor[c] - balance[BLACK][c])) / ((float) balance[GREY][c]) * 255.0;

    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    delay(RGBWait);
  }

  Serial.print("Current color: ");
  for (int i = 0; i < 3; i++) {
    Serial.print(currentColor[i]);
    Serial.print(" ");
  }
  Serial.println("");
}

void setBalance() {
  String title[2] = { "White" , "Black" };

  for (int c = 0; c < 2; c++) {
    // Start countdown to place color below sensor
    Serial.println("Put " + title[c] + " Sample For Calibration in: ");
    for (int i = COUNTDOWN; i > 0; i--) {
      Serial.print(i);
      Serial.print(" ");
      delay(1000);
    }
    Serial.println("");

    // Get average LDR reading for R, G & B
    Serial.print(title[c] + ": ");
    for (int i = 0; i < 3; i++) {
      digitalWrite(D1, ledPins[i][0]);
      digitalWrite(D2, ledPins[i][1]);
      delay(RGBWait);

      for (int k = 0; k < 5; k++) getAvgReading();
      balance[c][i] = getAvgReading();

      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);
      delay(RGBWait);

      Serial.print(balance[c][i]);
      Serial.print(", ");
    }
    Serial.println("\n");
  }

  Serial.print("GREY range: ");
  for (int j = 0; j < 3; j++) {
    balance[GREY][j] = balance[WHITE][j] - balance[BLACK][j];
    Serial.print(balance[GREY][j]);
    Serial.print(",");
  }
  Serial.println("\n");
}

void getColorRanges() {
  String title[5] = { "Red" , "Green" , "Orange" , "Purple" , "Blue" };
  for (int c = 0; c < 5; c++) {
    // Start countdown to place color below sensor
    Serial.println("Put " + title[c] + " Sample For Calibration in: ");
    for (int i = COUNTDOWN; i > 0; i--) {
      Serial.print(i);
      Serial.print(" ");
      delay(1000);
    }
    Serial.println("");

    // Get average LDR reading for R, G & B
    Serial.print(title[c] + ": ");
    for (int i = 0; i < 3; i++) {
      digitalWrite(D1, ledPins[i][0]);
      digitalWrite(D2, ledPins[i][1]);
      delay(RGBWait);

      getAvgReading();
      finalColors[c][i] = getAvgReading();
      finalColors[c][i] = ((float) (finalColors[c][i] - balance[BLACK][i])) / ((float) balance[GREY][i]) * 255.0;

      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);
      delay(RGBWait);

      Serial.print(finalColors[c][i]);
      Serial.print(", ");
    }
    Serial.println("\n");
  }
}

void writeToEEPROM() {
  int eeAddress = 0;

  Serial.println("Writing balance values to EEPROM...");
  for (int b = 0; b < 3; b++) {
    for (int c = 0; c < 3; c++) {
      EEPROM.put(eeAddress, balance[b][c]);
      eeAddress += sizeof(int);
    }
  }

  EEPROM.put(eeAddress, 'J');
  EEPROM.put(eeAddress + 1, 'B');
  EEPROM.put(eeAddress + 2, 'M');
}

int getAvgReading() {
  int total = 0;

  for (int i = 0; i < AVG_READING; i++) {
    int reading = analogRead(LDR);
    total += reading;
    delay(LDRWait);
  }

  return total / AVG_READING;
}
