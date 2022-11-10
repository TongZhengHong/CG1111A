/*
  d_sensor.ino: take in values from sensors & output readings to be used for navigation & colour sensing
*/

/* Ultrasonic Sensor */
#define TIMEOUT 2000        // Max microseconds to wait; choose according to max distance of wall
#define SPEED_OF_SOUND 340  // Metres per second
#define ULTRASONIC 12

float get_ultrasonic_distance() {
  pinMode(ULTRASONIC, OUTPUT);

  digitalWrite(ULTRASONIC, LOW);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC, LOW);

  pinMode(ULTRASONIC, INPUT);
  long duration = pulseIn(ULTRASONIC, HIGH, TIMEOUT); // Microseconds

  // m -> cm ( * 100) and microsecond -> second ( / 1000000)
  float conversion = 100.0 / 1000000.0;
  float distance_cm = duration * SPEED_OF_SOUND * 0.5 * conversion;

#ifndef DEBUG_ULTRASONIC
  Serial.print("Raw distance: ");
  Serial.print(distance_cm);
  Serial.println("");
#endif

  return distance_cm;
}

/* Colour Sensor */
int ledPins[4][2] = { // Logic gate for LED pins
  { HIGH, HIGH },     // Red LED
  { LOW, HIGH },      // Green LED
  { HIGH, LOW },      // Blue LED
  { LOW, LOW }        // IR sensor
};

#define LDR_PIN 0
#define RGBWait 100
#define LDRWait 10
#define AVG_READING 5 

void read_color_sensor() { 
  for (int c = 0; c < 3; c++) { 
    digitalWrite(D1, ledPins[c][0]);
    digitalWrite(D2, ledPins[c][1]);
    delay(RGBWait);

    // Get average LDR reading; 
    int total = 0;
    for (int i = 0; i < AVG_READING; i++) {
      int reading = analogRead(LDR_PIN);
      total += reading;
      delay(LDRWait);
    }

    currentColor[c] =  total / AVG_READING;
    currentColor[c] = ((float) (currentColor[c] - balanceArray[black][c])) / ((float) balanceArray[grey][c]) * 255.0;
    if (currentColor[c] < 0) currentColor[c] = 1; // Prevents a negative reading

    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    delay(RGBWait);
  }
  
#ifndef DEBUG_COLOR
  Serial.print("Current color: ");
  for (int i = 0; i < 3; i++) {
    Serial.print(currentColor[i]);
    Serial.print(" ");
  }
  Serial.println("");
#endif 
}

/* IR Sensor */
#define IR_PIN 1
int get_infrared_distance() {
  // Get average LDR reading
  int total = 0;
  for (int i = 0; i < AVG_READING; i++) {
    int reading = analogRead(IR_PIN);
    total += reading;
    delay(10);
  }
  total /= AVG_READING;
  
  return 0;
}

/* Check if mBot line sensor has detected black line on the floor */
bool has_reached_waypoint() {
  int sensor_state = line_finder.readSensors();
  return sensor_state == S1_IN_S2_IN;
}
