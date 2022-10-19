#define TIMEOUT 2000 // Max microseconds to wait; choose according to max distance of wall
#define SPEED_OF_SOUND 340 // metres per second

#define ULTRASONIC 12

float get_ultrasonic_distance() {
  pinMode(ULTRASONIC, OUTPUT);

  digitalWrite(ULTRASONIC, LOW);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC, LOW);

  pinMode(ULTRASONIC, INPUT);
  long duration = pulseIn(ULTRASONIC, HIGH, TIMEOUT); // microseconds

  // m -> cm ( * 100) and microsecond -> second ( / 1000000)
  float conversion = 100.0 / 1000000.0;
  float distance_cm = duration * SPEED_OF_SOUND * 0.5 * conversion;

#ifndef DEBUG_ULTRASONIC
  Serial.print("Ultrasonic distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
#endif

  return distance_cm;
}

int ledPins[3][2] = {
  { HIGH, HIGH },   // Red LED
  { HIGH, LOW },    // Green LED
  { LOW, HIGH }     // Blue LED
};

#define LDR 3
#define LED 13

#define RGBWait 200
#define LDRWait 10
#define AVG_READING 5

void read_color_sensor() {
  for (int c = 0; c < 3; c++) {
    digitalWrite(A0, ledPins[c][0]);
    digitalWrite(A1, ledPins[c][1]);
    delay(RGBWait);

    // Get average LDR reading
    int total = 0;
    for (int i = 0; i < AVG_READING; i++) {
      int reading = analogRead(LDR);
      total += reading;
      delay(LDRWait);
    }

    currentColor[c] =  total / AVG_READING;
    currentColor[c] = ((float) (currentColor[c] - balanceArray[BLACK][c])) / ((float) balanceArray[GREY][c]) * 255.0;

    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
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

int get_infrared_distance() {
  return 0;
}

bool has_reached_waypoint() {
  int sensor_state = line_finder.readSensors();
  return sensor_state == S1_IN_S2_IN;
}
